#include "DamageCalculator.h"

UDamageCalculator::UDamageCalculator()
{
}

UDamageCalculator::~UDamageCalculator()
{
}

FDamageResult UDamageCalculator::CalcDamage(const UBattler* _Attacker, const UBattler* _Defender)
{
    FDamageResult DamageResult;
    const FPokemonMove* Move = _Attacker->CurMove();
    const UPokemon* AttackerPokemon = _Attacker->CurPokemonReadonly();
    const UPokemon* DefenderPokemon = _Defender->CurPokemonReadonly();

    // 공격기가 아닌 경우 데미지 계산을 하지 않는다.
    if (Move->BasePower == 0)
    {
        DamageResult.Damage = 0;
        DamageResult.IsCritical = false;
        DamageResult.TypeVs = ETypeVs::NormallyEffective;
        return DamageResult;
    }

    bool IsCritical = CheckCritical(_Attacker);
    DamageResult.IsCritical = IsCritical;

    float DamageValue = 2.0f * AttackerPokemon->GetLevel() / 5.0f + 2.0f;
    DamageValue = std::floor(DamageValue);

    DamageValue *= GetEffectivePower(_Attacker);
    if (true == Move->IsPhysical())
    {
        DamageValue *= GetEffectiveAtk(_Attacker, IsCritical);
        DamageValue /= GetEffectiveDef(_Defender, IsCritical);
    }
    else
    {
        DamageValue *= GetEffectiveSpAtk(_Attacker, IsCritical);
        DamageValue /= GetEffectiveSpDef(_Defender, IsCritical);
    }
    DamageValue = std::floor(DamageValue);

    DamageValue /= 50.0f;
    DamageValue = std::floor(DamageValue);

    // 특성이 Guts(끈기)가 아닌 포켓몬이 화상 상태에서 물리 공격을 할 경우 데미지가 0.5배가 된다.
    if (AttackerPokemon->GetStatusId() == EPokemonStatus::Burn
        && true == Move->IsPhysical()
        && AttackerPokemon->GetAbilityId() != EPokemonAbility::Guts
    )
    {
        DamageValue *= 0.5f;
    }
    // Screen(Reflect, LightScreen), Weather, FF 미구현
    DamageValue = std::floor(DamageValue);

    if (true == Move->IsPhysical() && true == UPokemonMath::Equal(DamageValue, 0.0f))
    {
        DamageValue = 1.0f;
    }
    DamageValue += 2;

    // 크리티컬
    if (true == IsCritical)
    {
        DamageValue *= 2.0f;
    }

    // STAB(자속보정)
    if (true == AttackerPokemon->HasType(Move->TypeId))
    {
        DamageValue *= 1.5f;
    }
    DamageValue = std::floor(DamageValue);

    // 타입 효과
    const FPokemonType* MoveType = UPokemonDB::FindType(Move->TypeId);
    
    float TypeEffect = 1.0f;

    for (int i = 0; i < DefenderPokemon->GetTypeCount(); ++i)
    {
        EPokemonType TypeId = DefenderPokemon->GetTypeId(i);
        TypeEffect *= MoveType->EffectTo.at(TypeId);
    }
    DamageValue *= TypeEffect;
    DamageValue = std::floor(DamageValue);

    if (true == UPokemonMath::Equal(TypeEffect, 0.0f))
    {
        DamageResult.TypeVs = ETypeVs::HasNoEffect;
    }
    else if (true == UPokemonMath::Equal(TypeEffect, 0.5f) || true == UPokemonMath::Equal(TypeEffect, 0.25f))
    {
        DamageResult.TypeVs = ETypeVs::NotVeryEffective;
    }
    else if (true == UPokemonMath::Equal(TypeEffect, 1.0f))
    {
        DamageResult.TypeVs = ETypeVs::NormallyEffective;
    }
    else
    {
        DamageResult.TypeVs = ETypeVs::SuperEffective;
    }

    // 랜덤 변수
    int RandomNumber = UPokemonMath::RandomInt(217, 255);
    DamageValue *= RandomNumber;
    DamageValue /= 255;

    DamageResult.Damage = UPokemonMath::Floor(DamageValue);

    return DamageResult;
}

bool UDamageCalculator::CheckCritical(const UBattler* _Attacker)
{
    int C = 0;
    // 현재까지의 구현 내용 상으로는 딱히 C값을 증가시킬 요소가 없다.
    
    float Prob = 0.0f;
    
    if (C == 0)
    {
        Prob = 1.0f / 16;
    }
    else if (C == 1)
    {
        Prob = 1.0f / 8;
    }
    else if (C == 2)
    {
        Prob = 1.0f / 4;
    }
    else if (C == 3)
    {
        Prob = 1.0f / 3;
    }
    else if (C >= 4)
    {
        Prob = 1.0f / 2;
    }

    float RandomNumber = UPokemonMath::Random(0.0f, 1.0f);
    return RandomNumber < Prob;
}

float UDamageCalculator::GetEffectivePower(const UBattler* _Attacker)
{
    const FPokemonMove* Move = _Attacker->CurMove();
    const UPokemon* AttackerPokemon = _Attacker->CurPokemonReadonly();
    float Power = static_cast<float>(Move->BasePower);

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (AttackerPokemon->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && AttackerPokemon->GetCurHp() <= UPokemonMath::Floor(AttackerPokemon->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (AttackerPokemon->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && AttackerPokemon->GetCurHp() <= UPokemonMath::Floor(AttackerPokemon->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Torrent 특성은 체력이 1/3 이하일 때 물 타입 기술의 Power를 1.5배로 늘려준다.
    if (AttackerPokemon->GetAbilityId() == EPokemonAbility::Torrent
        && Move->TypeId == EPokemonType::Water
        && AttackerPokemon->GetCurHp() <= UPokemonMath::Floor(AttackerPokemon->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Blaze 특성은 체력이 1/3 이하일 때 불 타입 기술의 Power를 1.5배로 늘려준다.
    if (AttackerPokemon->GetAbilityId() == EPokemonAbility::Blaze
        && Move->TypeId == EPokemonType::Fire
        && AttackerPokemon->GetCurHp() <= UPokemonMath::Floor(AttackerPokemon->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    return Power;
}

float UDamageCalculator::GetEffectiveAtk(const UBattler* _Battler, bool _IsCritical)
{
    const UPokemon* Pokemon = _Battler->CurPokemonReadonly();
    const UStatStage& StatStage = _Battler->StatStage;

    float SummaryValue = static_cast<float>(Pokemon->GetAtk());
    float Multiplier = StatStage.GetAtkMultiplier();

    if (true == _IsCritical && StatStage.GetAtk() < 0)
    {
        // 크리티컬 공격은 공격자의 음수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    if (Pokemon->GetAbilityId() == EPokemonAbility::Guts && Pokemon->GetStatusId() != EPokemonStatus::Normal)
    {
        Multiplier *= 1.5f;
    }

    return SummaryValue * Multiplier;
}

float UDamageCalculator::GetEffectiveSpAtk(const UBattler* _Battler, bool _IsCritical)
{
    const UPokemon* Pokemon = _Battler->CurPokemonReadonly();
    const UStatStage& StatStage = _Battler->StatStage;

    float SummaryValue = static_cast<float>(Pokemon->GetSpAtk());
    float Multiplier = StatStage.GetSpAtkMultiplier();

    if (true == _IsCritical && StatStage.GetSpAtk() < 0)
    {
        // 크리티컬 공격은 공격자의 음수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}

float UDamageCalculator::GetEffectiveDef(const UBattler* _Battler, bool _IsCritical)
{
    const UPokemon* Pokemon = _Battler->CurPokemonReadonly();
    const UStatStage& StatStage = _Battler->StatStage;

    float SummaryValue = static_cast<float>(Pokemon->GetDef());
    float Multiplier = StatStage.GetDefMultiplier();

    if (true == _IsCritical && StatStage.GetDef() > 0)
    {
        // 크리티컬 공격은 방어자의 양수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}


float UDamageCalculator::GetEffectiveSpDef(const UBattler* _Battler, bool _IsCritical)
{
    const UPokemon* Pokemon = _Battler->CurPokemonReadonly();
    const UStatStage& StatStage = _Battler->StatStage;

    float SummaryValue = static_cast<float>(Pokemon->GetSpDef());
    float Multiplier = StatStage.GetSpDefMultiplier();

    if (true == _IsCritical && StatStage.GetSpDef() > 0)
    {
        // 크리티컬 공격은 방어자의 양수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}
