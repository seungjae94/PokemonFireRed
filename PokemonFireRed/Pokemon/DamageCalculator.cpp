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

    // ���ݱⰡ �ƴ� ��� ������ ����� ���� �ʴ´�.
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

    // Ư���� Guts(����)�� �ƴ� ���ϸ��� ȭ�� ���¿��� ���� ������ �� ��� �������� 0.5�谡 �ȴ�.
    if (_Attacker->GetStatusId() == EPokemonStatus::Burn
        && true == Move->IsPhysical()
        && _Attacker->GetAbilityId() != EPokemonAbility::Guts
    )
    {
        DamageValue *= 0.5f;
    }
    // Screen(Reflect, LightScreen), Weather, FF �̱���
    DamageValue = std::floor(DamageValue);

    if (true == Move->IsPhysical() && true == UPokemonMath::Equal(DamageValue, 0.0f))
    {
        DamageValue = 1.0f;
    }
    DamageValue += 2;

    // ũ��Ƽ��
    if (true == IsCritical)
    {
        DamageValue *= 2.0f;
    }

    // STAB(�ڼӺ���)
    if (true == _Attacker->HasType(Move->TypeId))
    {
        DamageValue *= 1.5f;
    }
    DamageValue = std::floor(DamageValue);

    // Ÿ�� ȿ��
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

    // ���� ����
    float RandomNumber = UPokemonMath::Random(0.85f, 1.0f);
    DamageValue *= RandomNumber;

    DamageResult.Damage = UPokemonMath::Floor(DamageValue);

    return DamageResult;
}

bool UDamageCalculator::CheckCritical(const UPokemon* _PlayerPokemon, EPokemonMove _MoveId)
{
    int C = 0;
    // ��������� ���� ���� �����δ� ���� C���� ������ų ��Ұ� ����.
    
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

    // Overgrow Ư���� ü���� 1/3 ������ �� Ǯ Ÿ�� ����� Power�� 1.5��� �÷��ش�.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow Ư���� ü���� 1/3 ������ �� Ǯ Ÿ�� ����� Power�� 1.5��� �÷��ش�.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow Ư���� ü���� 1/3 ������ �� Ǯ Ÿ�� ����� Power�� 1.5��� �÷��ش�.
    if (_Attacker->GetAbilityId() == EPokemonAbility::Overgrow
        && Move->TypeId == EPokemonType::Grass
        && _Attacker->GetCurHp() <= UPokemonMath::Floor(_Attacker->GetHp() / 3.0f))
    {
        Power *= 1.5f;
    }

    // Overgrow Ư���� ü���� 1/3 ������ �� Ǯ Ÿ�� ����� Power�� 1.5��� �÷��ش�.
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
        // ũ��Ƽ�� ������ �������� ���� Stat Stage�� �����Ѵ�.
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
        // ũ��Ƽ�� ������ �������� ���� Stat Stage�� �����Ѵ�.
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
        // ũ��Ƽ�� ������ ������� ��� Stat Stage�� �����Ѵ�.
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
        // ũ��Ƽ�� ������ ������� ��� Stat Stage�� �����Ѵ�.
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}
