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
    bool IsCritical = CheckCritical(_Attacker, _MoveId);
    DamageResult.IsCritical = IsCritical;

    const FPokemonMove* Move = UPokemonDB::FindMove(_MoveId);

    float DamageValue = 2.0f * _Attacker->GetLevel() / 5.0f + 2.0f;
    DamageValue = std::floor(DamageValue);
    DamageValue *= Move->BasePower;

    /*if (true == Move->IsPhysical())
    {
        DamageValue *= GetEffectiveAtk(_Attacker, _AttackerStatStage, IsCritical);
        DamageValue /= GetEffectiveDef(_Defender, _DefenderStatStage, IsCritical);
    }
    else
    {
        DamageValue *= GetEffectiveSpAtk(_Attacker, _AttackerStatStage, IsCritical);
        DamageValue /= GetEffectiveSpDef(_Defender, _DefenderStatStage, IsCritical);
    }*/

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

float UDamageCalculator::GetEffectiveAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical)
{
    float SummaryValue = static_cast<float>(_Pokemon->GetAtk());
    float Multiplier = _StatStage.GetAtkMultiplier();

    if (true == _IsCritical && _StatStage.GetAtk() < 0)
    {
        Multiplier = 1.0f;
    }

    return SummaryValue * Multiplier;
}
