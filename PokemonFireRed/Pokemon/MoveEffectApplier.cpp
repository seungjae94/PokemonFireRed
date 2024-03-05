#include "MoveEffectApplier.h"

UMoveEffectApplier::UMoveEffectApplier()
{
}

UMoveEffectApplier::~UMoveEffectApplier()
{
}

void UMoveEffectApplier::ApplyBE(UBattler* _Attacker, UBattler* _Defender)
{
	const FPokemonMove* Move = _Attacker->CurMove();

	UBattler* Target = nullptr;
	if (Move->BETarget == EMoveEffectTarget::Self)
	{
		Target = _Attacker;
	}
	else
	{
		Target = _Defender;
	}

	EStatStageChangeType StatStageId = Move->BEStatStageId;
	int StatStageValue = Move->BEStatStageValue;
	EPokemonStatus StatusId = Move->BEStatusId;

	Apply(Target, StatStageId, StatStageValue, StatusId);
}

void UMoveEffectApplier::ApplySE(UBattler* _Attacker, UBattler* _Defender)
{
	const FPokemonMove* Move = _Attacker->CurMove();

	UBattler* Target = nullptr;
	if (Move->SETarget == EMoveEffectTarget::Self)
	{
		Target = _Attacker;
	}
	else
	{
		Target = _Defender;
	}

	EStatStageChangeType StatStageId = Move->SEStatStageId;
	int StatStageValue = Move->SEStatStageValue;
	EPokemonStatus StatusId = Move->SEStatusId;

	Apply(Target, StatStageId, StatStageValue, StatusId);
}

void UMoveEffectApplier::Apply(UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue, EPokemonStatus _StatusId)
{
	if (_StatStageId != EStatStageChangeType::None)
	{
		UStatStage& TargetStatStage = _Target->StatStage;

		switch (_StatStageId)
		{
		case EStatStageChangeType::Atk:
			TargetStatStage.AddAtk(_StatStageValue);
			break;
		case EStatStageChangeType::Def:
			TargetStatStage.AddDef(_StatStageValue);
			break;
		case EStatStageChangeType::SpAtk:
			TargetStatStage.AddSpAtk(_StatStageValue);
			break;
		case EStatStageChangeType::SpDef:
			TargetStatStage.AddSpDef(_StatStageValue);
			break;
		case EStatStageChangeType::Speed:
			TargetStatStage.AddSpeed(_StatStageValue);
			break;
		case EStatStageChangeType::Accuracy:
			TargetStatStage.AddAccuracy(_StatStageValue);
			break;
		case EStatStageChangeType::Evasion:
			TargetStatStage.AddEvasion(_StatStageValue);
			break;
		default:
			break;
		}
	}

	if (_StatusId != EPokemonStatus::Normal)
	{
		switch (_StatusId)
		{
		case EPokemonStatus::Sleep:
		case EPokemonStatus::Poison:
		case EPokemonStatus::Burn:
		case EPokemonStatus::Freeze:
		case EPokemonStatus::Paralysis:
			_Target->CurPokemon()->SetStatus(_StatusId);
			break;
		case EPokemonStatus::TempSeeded:
		case EPokemonStatus::TempBound:
			_Target->SetTempStatus(_StatusId);
			break;
		default:
			break;
		}
	}
}
