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

	// 스탯 증감 성공 여부 확인
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

		// 이미 스탯이 6이거나 -6일 경우 실패
		if (NextStatStage == PrevStatStage)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatStageCap;
			Result.Message = GenerateStatStageFailMessage(_Target, _StatStageId, _StatStageValue);
			return Result;
		}
	}

	// 상태 적용 체크
	if (_StatusId != EPokemonStatus::Normal)
	{
		const FPokemonStatus* MoveStatus = UGameData::FindStatus(_StatusId);
		EPokemonStatus TargetStatusId = _Target->CurStatusId();
		EPokemonStatus TargetTempStatusId = _Target->CurTempStatusId();

		bool IsMoveApplyTempStatus = MoveStatus->IsTempStatus();
		// TempStatus를 적용하려고 하는데 이미 TempStatus가 있다면 실패 (수정 가능성 있음)
		if (true == IsMoveApplyTempStatus && TargetTempStatusId != EPokemonStatus::Normal)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatusOverlap;
			Result.Message = GenerateStatusFailMessage(_Attacker);
			return Result;
		}
		// Status를 적용하려고 하는데 이미 Status가 있다면 실패
		else if (false == IsMoveApplyTempStatus && TargetStatusId != EPokemonStatus::Normal)
		{
			Result.Success = false;
			Result.Reason = EMoveEffectTestFailureReason::StatusOverlap;
			Result.Message = GenerateStatusFailMessage(_Attacker);
			return Result;
		}
	}

	// 난수 테스트 성공
	int RandomNumber = UPokemonMath::RandomInt(0, 99);
	if (RandomNumber < _SuccessRate)
	{
		Result.Success = true;

		if (_StatStageId != EStatStageChangeType::None)
		{
			Result.Message = GenerateStatStageSuccessMessage(_Target, _StatStageId, _StatStageValue);
		}
		else
		{
			Result.Message = GenerateStatusSuccessMessage(_Target, _StatusId);
		}
		return Result;
	}

	// 난수 테스트 실패
	Result.Success = false;
	Result.Reason = EMoveEffectTestFailureReason::RandomTest;
	Result.Message = UBattleUtil::GetPokemonFullName(_Attacker) + L"'s\nattack failed!";
	return Result;
}

std::wstring UMoveEffectTester::GenerateStatStageFailMessage(const UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue)
{
	std::wstring Message = UBattleUtil::GetPokemonFullName(_Target) + L"'s ";
	Message += UBattleUtil::GetStatStageNameW(_StatStageId);

	if (_StatStageValue > 0)
	{
		Message += L"\nwon't go higher!";
	}
	else
	{
		Message += L"\nwon't go lower!";
	}

	return Message;
}

std::wstring UMoveEffectTester::GenerateStatStageSuccessMessage(const UBattler* _Target, EStatStageChangeType _StatStageId, int _StatStageValue)
{
	std::wstring Message = UBattleUtil::GetPokemonFullName(_Target) + L"'s ";
	Message += UBattleUtil::GetStatStageNameW(_StatStageId) + L"\n";
	Message += UBattleUtil::GetStatStageChangeMessageSuffix(_StatStageValue);
	return Message;
}

std::wstring UMoveEffectTester::GenerateStatusSuccessMessage(const UBattler* _Target, EPokemonStatus _StatusId)
{
	std::wstring Message = UBattleUtil::GetPokemonFullName(_Target) + L" was ";
	switch (_StatusId)
	{
	case EPokemonStatus::Sleep:
		Message += L"sleeped!";
		break;
	case EPokemonStatus::Poison:
		Message += L"poisoned!";
		break;
	case EPokemonStatus::Burn:
		Message += L"burned!";
		break;
	case EPokemonStatus::Freeze:
		Message += L"freezed!";
		break;
	case EPokemonStatus::Paralysis:
		Message += L"paralyzed!";
		break;
	case EPokemonStatus::TempSeeded:
		Message += L"seeded!";
		break;
	case EPokemonStatus::TempBound:
		Message += L"bound!";
		break;
	default:
		break;
	}
	return Message;
}

std::wstring UMoveEffectTester::GenerateStatusFailMessage(const UBattler* _Attacker)
{
	return UBattleUtil::GetPokemonFullName(_Attacker) + L"'s\nattack failed!";
}
