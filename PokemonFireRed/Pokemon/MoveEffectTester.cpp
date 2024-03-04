#include "MoveEffectTester.h"
#include "BattleUtil.h"

UMoveEffectTester::UMoveEffectTester()
{
}

UMoveEffectTester::~UMoveEffectTester()
{
}

FMoveEffectTestResult UMoveEffectTester::TestBE(const UBattler* _Attacker, const UBattler* _Defender)
{
	const FPokemonMove* Move = _Attacker->CurMove();

	const UBattler* Target = nullptr;
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
	int SuccessRate = Move->Accuracy;
	return Test(_Attacker, Target, StatStageId, StatStageValue, StatusId, SuccessRate, Move->BETarget);
}

FMoveEffectTestResult UMoveEffectTester::TestSE(const UBattler* _Attacker, const UBattler* _Defender)
{
	const FPokemonMove* Move = _Attacker->CurMove();

	const UBattler* Target = nullptr;
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
	int SuccessRate = Move->SERate;
	return Test(_Attacker, Target, StatStageId, StatStageValue, StatusId, SuccessRate, Move->SETarget);
}

FMoveEffectTestResult UMoveEffectTester::Test(const UBattler* _Attacker, const UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue, EPokemonStatus _StatusId, int _SuccessRate, EMoveEffectTarget _TargetType)
{
	FMoveEffectTestResult Result;

	// ���� ���� ���� ���� Ȯ��
	if (_StatStageId != EStatStageChangeType::None)
	{
		const UStatStage& TargetStatStage = _Target->StatStage;

		int PrevStatStage = 0;
		switch (_StatStageId)
		{
		case EStatStageChangeType::Atk:
			PrevStatStage = TargetStatStage.GetAtk();
			break;
		case EStatStageChangeType::Def:
			PrevStatStage = TargetStatStage.GetDef();
			break;
		case EStatStageChangeType::SpAtk:
			PrevStatStage = TargetStatStage.GetSpAtk();
			break;
		case EStatStageChangeType::SpDef:
			PrevStatStage = TargetStatStage.GetSpDef();
			break;
		case EStatStageChangeType::Speed:
			PrevStatStage = TargetStatStage.GetSpeed();
			break;
		case EStatStageChangeType::Accuracy:
			PrevStatStage = TargetStatStage.GetAccuracy();
			break;
		case EStatStageChangeType::Evasion:
			PrevStatStage = TargetStatStage.GetEvasion();
			break;
		default:
			break;
		}

		int NextStatStage = UPokemonMath::Cap(PrevStatStage + _StatStageValue, -6, 6);

		// �̹� ������ 6�̰ų� -6�� ��� ����
		if (NextStatStage == PrevStatStage)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatStageCap;

			Result.Message = L"";
			Result.Message = UBattleUtil::GetPokemonFullName(_Target);
			Result.Message += L" " + UBattleUtil::GetStatStageNameW(_StatStageId);

			if (_StatStageValue > 0)
			{
				Result.Message += L"\nwon't go lower!";
			}
			else
			{
				Result.Message += L"\nwon't go higher!";
			}

			return Result;
		}
	}

	// ���¸� ���� üũ
	if (_StatusId != EPokemonStatus::None)
	{
		const FPokemonStatus* MoveStatus = UPokemonDB::FindStatus(_StatusId);
		const FPokemonStatus* TargetStatus = _Target->CurStatus();
		const FPokemonStatus* TargetTempStatus = _Target->CurTempStatus();

		bool IsMoveApplyTempStatus = MoveStatus->IsTempStatus();
		// TempStatus�� �����Ϸ��� �ϴµ� �̹� TempStatus�� �ִٸ� ����
		if (true == IsMoveApplyTempStatus && TargetTempStatus->Id == EPokemonStatus::None)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatusOverlap;
			Result.Message = UBattleUtil::GetPokemonFullName(_Attacker) + L"'s\nattack failed!";
			return Result;
		}
		// Status�� �����Ϸ��� �ϴµ� �̹� Status�� �ִٸ� ����
		else if (false == IsMoveApplyTempStatus && TargetStatus->Id == EPokemonStatus::None)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatusOverlap;
			Result.Message = UBattleUtil::GetPokemonFullName(_Attacker) + L"'s\nattack failed!";
			return Result;
		}
	}

	// ���� �׽�Ʈ ����
	int RandomNumber = UPokemonMath::RandomInt(0, 99);
	if (RandomNumber < _SuccessRate)
	{
		Result.Success = true;
		return Result;
	}

	// ���� �׽�Ʈ ����
	Result.Success = false;
	Result.Reason = EMoveEffectTestFailureReason::RandomTest;
	Result.Message = UBattleUtil::GetPokemonFullName(_Attacker) + L"'s\nattack failed!";
	return Result;
}