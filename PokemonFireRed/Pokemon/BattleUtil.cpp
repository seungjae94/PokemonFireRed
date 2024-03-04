#include "BattleUtil.h"

UBattleUtil::UBattleUtil()
{
}

UBattleUtil::~UBattleUtil()
{
}

std::wstring UBattleUtil::GetPokemonFullName(const UBattler* _Battler)
{
	std::wstring PokemonName = _Battler->CurPokemonReadonly()->GetNameW();

	if (true == _Battler->IsWildPokemon())
	{
		return L"Wild " + PokemonName;
	}
	else if (true == _Battler->IsPlayer())
	{
		return PokemonName;
	}

	return L"Foe " + PokemonName;
}

std::wstring UBattleUtil::GetStatStageNameW(EStatStageChangeType _StatStageId)
{
	switch (_StatStageId)
	{
	case EStatStageChangeType::Atk:
		return L"ATTACK";
	case EStatStageChangeType::Def:
		return L"DEFENSE";
	case EStatStageChangeType::SpAtk:
		return L"SP.ATK";
	case EStatStageChangeType::SpDef:
		return L"SP.DEF";
	case EStatStageChangeType::Speed:
		return L"SPEED";
	case EStatStageChangeType::Accuracy:
		return L"accuracy";
	case EStatStageChangeType::Evasion:
		return L"evasiveness";
	default:
		return std::wstring();
	}
}

std::wstring UBattleUtil::GetStatStageChangeMessageSuffix(int _Value)
{
	if (_Value == 1)
	{
		return L"rose!";
	}
	else if (_Value >= 2)
	{
		return L"sharply rose!";
	}
	else if (_Value == -1)
	{
		return L"fell!";
	}
	return L"harshly fell!";
}

