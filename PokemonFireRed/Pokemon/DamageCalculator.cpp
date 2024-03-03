#include "DamageCalculator.h"

UDamageCalculator::UDamageCalculator()
{
}

UDamageCalculator::~UDamageCalculator()
{
}

FDamageResult UDamageCalculator::CalcDamage(const UPokemon* _Attacker, const UPokemon* _Defender, const UStatStage& _AttackerStatStage, const UStatStage& _DefenderStatStage, EPokemonMove _MoveId)
{
    FDamageResult DamageResult;
    const FPokemonMove* Move = UPokemonDB::FindMove(_MoveId);

    // 공격기가 아닌 경우 데미지 계산을 하지 않는다.
    if (Move->BasePower == 0)
    {
        DamageResult.Damage = 0;
        DamageResult.IsCritical = false;
        DamageResult.TypeVs = ETypeVs::NormallyEffective;
        return DamageResult;
    }

    bool IsCritical = CheckCritical(_Attacker, _MoveId);
    DamageResult.IsCritical = IsCritical;

    float DamageValue = 2.0f * _Attacker->GetLevel() / 5.0f + 2.0f;
    DamageValue = std::floor(DamageValue);

    DamageValue *= GetEffectivePower(_Attacker, _MoveId);
    if (true == Move->IsPhysical())
    {
        DamageValue *= GetEffectiveAtk(_Attacker, _AttackerStatStage, IsCritical);
        DamageValue /= GetEffectiveDef(_Defender, _DefenderStatStage, IsCritical);
    }
    else
    {
        DamageValue *= GetEffectiveSpAtk(_Attacker, _AttackerStatStage, IsCritical);
        DamageValue /= GetEffectiveSpDef(_Defender, _DefenderStatStage, IsCritical);
    }
    DamageValue = std::floor(DamageValue);

    DamageValue /= 50.0f;
    DamageValue = std::floor(DamageValue);

    // 특성이 Guts(끈기)가 아닌 포켓몬이 화상 상태에서 물리 공격을 할 경우 데미지가 0.5배가 된다.
    if (_Attacker->GetStatusId() == EPokemonStatus::Burn
        && true == Move->IsPhysical()
        && _Attacker->GetAbilityId() != EPokemonAbility::Guts
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
    if (true == _Attacker->HasType(Move->TypeId))
    {
        DamageValue *= 1.5f;
    }
    DamageValue = std::floor(DamageValue);

    // 타입 효과
    const FPokemonType* MoveType = UPokemonDB::FindType(Move->TypeId);
    
    float TypeEffect = 1.0f;

    for (int i = 0; i < _Defender->GetTypeCount(); ++i)
    {
        EPokemonType TypeId = _Defender->GetTypeId(i);
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
    float RandomNumber = UPokemonMath::Random(0.85f, 1.0f);
    DamageValue *= RandomNumber;

    DamageResult.Damage = UPokemonMath::Floor(DamageValue);

    return DamageResult;
}

bool UDamageCalculator::CheckCritical(const UPokemon* _PlayerPokemon, EPokemonMove _MoveId)
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

float UDamageCalculator::GetEffectivePower(const UPokemon* _Attacker, EPokemonMove _MoveId)
{
    const FPokemonMove* Move = UPokemonDB::FindMove(_MoveId);
    float Power = static_cast<float>(Move->BasePower);

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow 특성은 체력이 1/3 이하일 때 풀 타입 기술의 Power를 1.5배로 늘려준다.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    return Power;
}

float UDamageCalculator::GetEffectiveAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical)
{
    float SummaryValue = static_cast<float>(_Pokemon->GetAtk());
    float Multiplier = _StatStage.GetAtkMultiplier();

    if (true == _IsCritical && _StatStage.GetAtk() < 0)
    {
        // 크리티컬 공격은 공격자의 음수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    if (_Pokemon->GetAbilityId() == EPokemonAbility::Guts && _Pokemon->GetStatusId() != EPokemonStatus::Normal)
    {
        Multiplier *= 1.5f;
    }

    return SummaryValue * Multiplier;
}

float UDamageCalculator::GetEffectiveSpAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical)
{
    float SummaryValue = static_cast<float>(_Pokemon->GetSpAtk());
    float Multiplier = _StatStage.GetSpAtkMultiplier();

    if (true == _IsCritical && _StatStage.GetSpAtk() < 0)
    {
        // 크리티컬 공격은 공격자의 음수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}

float UDamageCalculator::GetEffectiveDef(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical)
{
    float SummaryValue = static_cast<float>(_Pokemon->GetDef());
    float Multiplier = _StatStage.GetDefMultiplier();

    if (true == _IsCritical && _StatStage.GetDef() > 0)
    {
        // 크리티컬 공격은 방어자의 양수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}


float UDamageCalculator::GetEffectiveSpDef(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical)
{
    float SummaryValue = static_cast<float>(_Pokemon->GetSpDef());
    float Multiplier = _StatStage.GetSpDefMultiplier();

    if (true == _IsCritical && _StatStage.GetSpDef() > 0)
    {
        // 크리티컬 공격은 방어자의 양수 Stat Stage를 무시한다.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}
