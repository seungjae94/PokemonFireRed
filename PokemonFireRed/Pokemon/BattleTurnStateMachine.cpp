#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"
#include "Battler.h"
#include "BattleUtil.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy)
{
	// 입력 데이터 저장
	Canvas = _Canvas;
	Player = _Player;
	Enemy = _Enemy;

	// 적 액션 생성
	UBattleEnemyActionGenerator::Generate(Enemy);

	// 턴 순서 결정
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	IsFirstTurn = true;

	// 상태 변경
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}
	DispatchTurn();
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerEscapeFail:
		ProcessEscapeFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::Switch:
		break;
	case ABattleTurnStateMachine::ESubstate::UseItem:
		break;
	case ABattleTurnStateMachine::ESubstate::MoveFail:
		ProcessMoveFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveAnim:
		ProcessMoveAnim(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveDamage:
		ProcessMoveDamage(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveBattleEffect:
		ProcessMoveBattleEffect(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveSecondaryEffect:
		ProcessMoveSecondaryEffect(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::Faint:
		ProcessFaint(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn:
		ProcessEndOfTurn(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchTurn()
{
	EBattleAction AttackerAction = Attacker->CurAction();

	switch (AttackerAction)
	{
	case EBattleAction::Fight:
		DispatchFight();
		break;
	case EBattleAction::Escape:
	{
		State = ESubstate::PlayerEscapeFail;
		Canvas->SetBattleMessage(L"Can't Escape!");
	}
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::Switch;
		Canvas->SetBattleMessage(L"Debug - Attacker Changed Pokemon!");
	}
	break;
	case EBattleAction::Item:
	{
		State = ESubstate::UseItem;
		Canvas->SetBattleMessage(L"Debug - Attacker used Item!");
	}
	break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchFight()
{
	const FPokemonMove* Move = Attacker->CurMove();

	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender);

	// 정확도 체크 단계에서 실패할 경우 Move Fail
	if (false == AccuracyCheckResult)
	{
		std::wstring BattleMsg = UBattleUtil::GetPokemonFullName(Attacker) + L"'s\nattack failed!";
		StateChangeToMoveFail(BattleMsg);
		return;
	}

	// 공격기가 아닐 경우
	if (Move->BETarget != EMoveEffectTarget::None)
	{
		FMoveEffectTestResult TestResult = UMoveEffectTester::TestBE(Attacker, Defender);

		// 기술 사용에 실패한 경우 Move Fail
		if (false == TestResult.Success)
		{
			StateChangeToMoveFail(TestResult.Message);
			return;
		}
	}

	// 공격/BE 성공
	StateChangeToMoveAnim();
}

void ABattleTurnStateMachine::DispatchNextPhase()
{
	// Faint 처리
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetCurHp() == 0)
	{
		State = ESubstate::Faint;
		FaintState = EFaintState::None;
		return;
	}

	if (true == IsFirstTurn)
	{
		if (Attacker == Player)
		{
			SetEnemyAsAttacker();
		}
		else
		{
			SetPlayerAsAttacker();
		}
		IsFirstTurn = false;
		DispatchTurn();
		return;
	}

	// 후공이 끝난 경우
	DispatchEndOfTurn();
}

void ABattleTurnStateMachine::DispatchSecondaryEffect()
{
	const FPokemonMove* Move = Attacker->CurMove();
	FMoveEffectTestResult TestResult = UMoveEffectTester::TestSE(Attacker, Defender);

	// SE 대상이 자기 자신이고 테스트를 성공한 경우 SE 실행
	if (Move->SETarget == EMoveEffectTarget::Self && true == TestResult.Success)
	{
		StateChangeToMoveSecondaryEffect();
		return;
	}

	// Faint 상태일 경우 SE 스킵
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetCurHp() == 0)
	{
		DispatchNextPhase();
		return;
	}
	
	EMoveEffectTarget SETarget = Move->SETarget;
	// SE가 없거나 실패한 경우 조용히 다음 순서로 진행
	if (SETarget == EMoveEffectTarget::None || false == TestResult.Success)
	{
		DispatchNextPhase();
		return;
	}

	StateChangeToMoveSecondaryEffect();
}

void ABattleTurnStateMachine::DispatchEndOfTurn()
{
	// TODO: 누가 EOT를 먼저 실행할지 결정
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::DispatchFaintResult()
{
	if (Defender == Player)
	{
		// TODO: 다음 포켓몬을 꺼낼지 배틀에서 패배할지 계산하고 상태 변경
	}
	else if (true == Defender->IsWildPokemon())
	{
		// TODO: 배틀 승리 처리
	}
	else /*Defender is Traniner*/
	{
		// TODO: 다음 포켓몬을 꺼낼지 배틀에서 패배할지 계산하고 상태 변경
	}

	Canvas->SetBattleMessage(L"Debug - Faint Result");
}

void ABattleTurnStateMachine::ProcessEscapeFail(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		IsFirstTurn = false;
		SetEnemyAsAttacker();
		DispatchTurn();
		return;
	}
}

void ABattleTurnStateMachine::ProcessMoveFail(float _DeltaTime)
{
	if (Timer < 0.0f)
	{
		DispatchNextPhase();
	}
}

void ABattleTurnStateMachine::ProcessMoveAnim(float _DeltaTime)
{
	// 애니메이션: 차후 구현

	// 애니메이션 종료
	const FPokemonMove* Move = Attacker->CurMove();

	if (Move->BETarget == EMoveEffectTarget::None)
	{
		// 공격기인 경우
		StateChangeToMoveDamage();
	}
	else
	{
		// 공격기가 아닌 경우
		StateChangeToMoveBattleEffect();
	}
}

void ABattleTurnStateMachine::ProcessMoveDamage(float _DeltaTime)
{
	UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (MoveResultMsg == EMoveDamageState::None)
	{
		if (Defender == Player)
		{
			Canvas->LerpPlayerHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / DamageTime);
		}
		else
		{
			Canvas->LerpEnemyHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / DamageTime);
		}

		if (Timer <= 0.0f)
		{
			// 실제 데미지 처리
			DefenderPokemon->SetCurHp(NextHp);

			if (NextHp == 0)
			{
				DefenderPokemon->SetStatus(EPokemonStatus::Faint);
			}

			if (Result.IsCritical == true)
			{
				MoveResultMsg = EMoveDamageState::Critical;
				Canvas->SetBattleMessage(L"A critical hit!");
				Timer = BattleMsgShowTime;
				return;
			}
			else if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveDamageState::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
	else if (MoveResultMsg == EMoveDamageState::Critical)
	{
		if (Timer <= 0.0f)
		{
			if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveDamageState::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
	else /*MoveResultMsg == EMoveDamageState::TypeEffect*/
	{
		if (Timer <= 0.0f)
		{
			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveBattleEffect(float _DeltaTime)
{
	// (주의) BE 성공이 보장된 상태

	if (MoveEffectState == EMoveEffectState::None)
	{
		// 첫 틱인 경우 스탯, 상태 변경 로직을 처리하고 메시지를 준비한다.
		const FPokemonMove* Move = Attacker->CurMove();
		EStatStageChangeType BEStatStageId = Move->BEStatStageId;
		EPokemonStatus BEStatusId = Move->BEStatusId;

		if (BEStatStageId != EStatStageChangeType::None)
		{
			ChangeStatStage(Move->BETarget, BEStatStageId, Move->BEStatStageValue);
		}
		else if (BEStatusId != EPokemonStatus::None)
		{
			// 일단 스탯과 상태를 동시에 변경하는 경우는 없다고 가정하고 구현한다.
			ChangeStatus(Move->BETarget, BEStatusId);
		}

		MoveEffectState = EMoveEffectState::ShowMoveEffect;
		Timer = MoveEffectShowTime;
	}
	else if (MoveEffectState == EMoveEffectState::ShowMoveEffect)
	{
		// TODO: Battle Effect 애니메이션 재생

		if (Timer <= 0.0f)
		{
			MoveEffectState = EMoveEffectState::ShowEffectResultMessage;
			Timer = BattleMsgShowTime;
			Canvas->SetBattleMessage(MoveEffectMessage);
		}
	}
	else /*MoveEffectState == EMoveEffectState::ShowEffectResultMessage*/
	{
		if (Timer <= 0.0f)
		{
			DispatchSecondaryEffect();
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveSecondaryEffect(float _DeltaTime)
{
	if (MoveEffectState == EMoveEffectState::None)
	{
		// 첫 틱인 경우 스탯, 상태 변경 로직을 처리하고 메시지를 준비한다.
		const FPokemonMove* Move = Attacker->CurMove();
		EStatStageChangeType SEStatStageId = Move->SEStatStageId;
		EPokemonStatus SEStatusId = Move->SEStatusId;

		if (SEStatStageId != EStatStageChangeType::None)
		{
			ChangeStatStage(Move->SETarget, SEStatStageId, Move->SEStatStageValue);
		}
		else if (SEStatusId != EPokemonStatus::None)
		{
			// 일단 스탯과 상태를 동시에 변경하는 경우는 없다고 가정하고 구현한다.
			ChangeStatus(Move->SETarget, SEStatusId);
		}

		MoveEffectState = EMoveEffectState::ShowMoveEffect;
		Timer = MoveEffectShowTime;
	}
	else if (MoveEffectState == EMoveEffectState::ShowMoveEffect)
	{
		// TODO: Secondary Effect 애니메이션 재생

		if (Timer <= 0.0f)
		{
			MoveEffectState = EMoveEffectState::ShowEffectResultMessage;
			Timer = BattleMsgShowTime;
			Canvas->SetBattleMessage(MoveEffectMessage);
		}
	}
	else /*MoveEffectState == EMoveEffectState::ShowEffectResultMessage*/
	{
		if (Timer <= 0.0f)
		{
			DispatchNextPhase();
		}
	}
}

void ABattleTurnStateMachine::ProcessFaint(float _DeltaTime)
{
	if (FaintState == EFaintState::None)
	{
		FaintState = EFaintState::HidePokemon;
		Timer = FaintTime;
	}
	else if (FaintState == EFaintState::HidePokemon && Defender == Player)
	{
		Canvas->LerpFaintPlayerPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			FaintState = EFaintState::ShowFaintMessage;
			Canvas->SetPlayerPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Defender) + L"\nfainted!");
		}
	}
	else if (FaintState == EFaintState::HidePokemon && Defender == Enemy)
	{
		Canvas->LerpFaintEnemyPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			FaintState = EFaintState::ShowFaintMessage;
			Canvas->SetEnemyPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Defender) + L"\nfainted!");
		}
	}
	else if (/*FaintState == EFaintState::ShowFaintMessage && */true == UEngineInput::IsDown('Z'))
	{
		DispatchFaintResult();
	}
}


void ABattleTurnStateMachine::ProcessEndOfTurn(float _DeltaTime)
{
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);
	// Leech Seed 체크
	State = ESubstate::End;
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = Player;
	Defender = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = Enemy;
	Defender = Player;
}

void ABattleTurnStateMachine::ChangeStatStage(EMoveEffectTarget _METarget, EStatStageChangeType _MEStatStageId, int _MEStatStageValue)
{
	const FPokemonMove* Move = Attacker->CurMove();

	UBattler* Target = nullptr;
	if (_METarget == EMoveEffectTarget::Self)
	{
		Target = Attacker;
	}
	else
	{
		Target = Defender;
	}

	MoveEffectMessage = UBattleUtil::GetPokemonFullName(Target) + L"'s ";
	MoveEffectMessage += UBattleUtil::GetStatStageNameW(_MEStatStageId) + L"\n";

	int Value = _MEStatStageValue;
	switch (_MEStatStageId)
	{
	case EStatStageChangeType::Atk:
		Target->StatStage.AddAtk(Value);
		break;
	case EStatStageChangeType::Def:
		Target->StatStage.AddDef(Value);
		break;
	case EStatStageChangeType::SpAtk:
		Target->StatStage.AddSpAtk(Value);
		break;
	case EStatStageChangeType::SpDef:
		Target->StatStage.AddDef(Value);
		break;
	case EStatStageChangeType::Speed:
		Target->StatStage.AddSpeed(Value);
		break;
	case EStatStageChangeType::Accuracy:
		Target->StatStage.AddAccuracy(Value);
		break;
	case EStatStageChangeType::Evasion:
		Target->StatStage.AddEvasion(Value);
		break;
	default:
		break;
	}
	MoveEffectMessage += UBattleUtil::GetStatStageChangeMessageSuffix(Value);
}

void ABattleTurnStateMachine::ChangeStatus(EMoveEffectTarget _METarget, EPokemonStatus _MEStatus)
{
	const FPokemonMove* Move = Attacker->CurMove();
	const FPokemonStatus* Status = UPokemonDB::FindStatus(_MEStatus);

	UBattler* Target = nullptr;
	if (_METarget == EMoveEffectTarget::Self)
	{
		Target = Attacker;
	}
	else
	{
		Target = Defender;
	}
	UPokemon* TargetPokemon = Target->CurPokemon();

	MoveEffectMessage = UBattleUtil::GetPokemonFullName(Target);
	MoveEffectMessage += L" was ";

	switch (_MEStatus)
	{
	case EPokemonStatus::Sleep:
		MoveEffectMessage += L"sleeped!";
		TargetPokemon->SetStatus(_MEStatus);
		break;
	case EPokemonStatus::Poison:
		MoveEffectMessage += L"poisoned!";
		TargetPokemon->SetStatus(_MEStatus);
		break;
	case EPokemonStatus::Burn:
		MoveEffectMessage += L"burned!";
		TargetPokemon->SetStatus(_MEStatus);
		break;
	case EPokemonStatus::Freeze:
		MoveEffectMessage += L"freezed!";
		TargetPokemon->SetStatus(_MEStatus);
		break;
	case EPokemonStatus::Paralysis:
		MoveEffectMessage += L"paralyzed!";
		TargetPokemon->SetStatus(_MEStatus);
		break;
	case EPokemonStatus::TempSeeded:
		MoveEffectMessage += L"seeded!";
		Target->SetTempStatus(_MEStatus);
		break;
	case EPokemonStatus::TempBound:
		MoveEffectMessage += L"bound!";
		Target->SetTempStatus(_MEStatus);
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::StateChangeToMoveAnim()
{
	const FPokemonMove* Move = Attacker->CurMove();
	const UPokemon* AttackerPokemon = Attacker->CurPokemonReadonly();
	State = ESubstate::MoveAnim;
	Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	Result = UDamageCalculator::CalcDamage(Attacker, Defender);
}

void ABattleTurnStateMachine::StateChangeToMoveDamage()
{
	State = ESubstate::MoveDamage;
	Timer = DamageTime;
	MoveResultMsg = EMoveDamageState::None;
	PrevHp = Defender->CurPokemonReadonly()->GetCurHp();
	NextHp = UPokemonMath::Max(PrevHp - Result.Damage, 0);
}

void ABattleTurnStateMachine::StateChangeToMoveBattleEffect()
{
	State = ESubstate::MoveBattleEffect;
	MoveEffectState = EMoveEffectState::None;
	Timer = MoveEffectShowTime;
}

void ABattleTurnStateMachine::StateChangeToMoveSecondaryEffect()
{
	State = ESubstate::MoveSecondaryEffect;
	MoveEffectState = EMoveEffectState::None;
	Timer = MoveEffectShowTime;
}

void ABattleTurnStateMachine::StateChangeToMoveFail(std::wstring _FailMessage)
{
	Canvas->SetBattleMessage(_FailMessage);
	State = ESubstate::MoveFail;
	Timer = BattleMsgShowTime;
}
