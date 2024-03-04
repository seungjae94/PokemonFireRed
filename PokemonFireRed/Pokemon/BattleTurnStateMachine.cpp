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
	// �Է� ������ ����
	Canvas = _Canvas;
	Player = _Player;
	Enemy = _Enemy;

	// �� �׼� ����
	UBattleEnemyActionGenerator::Generate(Enemy);

	// �� ���� ����
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	IsFirstTurn = true;

	// ���� ����
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

	// ��Ȯ�� üũ �ܰ迡�� ������ ��� Move Fail
	if (false == AccuracyCheckResult)
	{
		std::wstring BattleMsg = UBattleUtil::GetPokemonFullName(Attacker) + L"'s\nattack failed!";
		StateChangeToMoveFail(BattleMsg);
		return;
	}

	// ���ݱⰡ �ƴ� ���
	if (Move->BETarget != EMoveEffectTarget::None)
	{
		FMoveEffectTestResult TestResult = UMoveEffectTester::TestBE(Attacker, Defender);

		// ��� ��뿡 ������ ��� Move Fail
		if (false == TestResult.Success)
		{
			StateChangeToMoveFail(TestResult.Message);
			return;
		}
	}

	// ����/BE ����
	StateChangeToMoveAnim();
}

void ABattleTurnStateMachine::DispatchNextPhase()
{
	// Faint ó��
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

	// �İ��� ���� ���
	DispatchEndOfTurn();
}

void ABattleTurnStateMachine::DispatchSecondaryEffect()
{
	const FPokemonMove* Move = Attacker->CurMove();
	FMoveEffectTestResult TestResult = UMoveEffectTester::TestSE(Attacker, Defender);

	// SE ����� �ڱ� �ڽ��̰� �׽�Ʈ�� ������ ��� SE ����
	if (Move->SETarget == EMoveEffectTarget::Self && true == TestResult.Success)
	{
		StateChangeToMoveSecondaryEffect();
		return;
	}

	// Faint ������ ��� SE ��ŵ
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetCurHp() == 0)
	{
		DispatchNextPhase();
		return;
	}
	
	EMoveEffectTarget SETarget = Move->SETarget;
	// SE�� ���ų� ������ ��� ������ ���� ������ ����
	if (SETarget == EMoveEffectTarget::None || false == TestResult.Success)
	{
		DispatchNextPhase();
		return;
	}

	StateChangeToMoveSecondaryEffect();
}

void ABattleTurnStateMachine::DispatchEndOfTurn()
{
	// TODO: ���� EOT�� ���� �������� ����
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::DispatchFaintResult()
{
	if (Defender == Player)
	{
		// TODO: ���� ���ϸ��� ������ ��Ʋ���� �й����� ����ϰ� ���� ����
	}
	else if (true == Defender->IsWildPokemon())
	{
		// TODO: ��Ʋ �¸� ó��
	}
	else /*Defender is Traniner*/
	{
		// TODO: ���� ���ϸ��� ������ ��Ʋ���� �й����� ����ϰ� ���� ����
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
	// �ִϸ��̼�: ���� ����

	// �ִϸ��̼� ����
	const FPokemonMove* Move = Attacker->CurMove();

	if (Move->BETarget == EMoveEffectTarget::None)
	{
		// ���ݱ��� ���
		StateChangeToMoveDamage();
	}
	else
	{
		// ���ݱⰡ �ƴ� ���
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
			// ���� ������ ó��
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

			// ������ ����
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

			// ������ ����
			DispatchSecondaryEffect();
		}
	}
	else /*MoveResultMsg == EMoveDamageState::TypeEffect*/
	{
		if (Timer <= 0.0f)
		{
			// ������ ����
			DispatchSecondaryEffect();
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveBattleEffect(float _DeltaTime)
{
	// (����) BE ������ ����� ����

	if (MoveEffectState == EMoveEffectState::None)
	{
		// ù ƽ�� ��� ����, ���� ���� ������ ó���ϰ� �޽����� �غ��Ѵ�.
		const FPokemonMove* Move = Attacker->CurMove();
		EStatStageChangeType BEStatStageId = Move->BEStatStageId;
		EPokemonStatus BEStatusId = Move->BEStatusId;

		if (BEStatStageId != EStatStageChangeType::None)
		{
			ChangeStatStage(Move->BETarget, BEStatStageId, Move->BEStatStageValue);
		}
		else if (BEStatusId != EPokemonStatus::None)
		{
			// �ϴ� ���Ȱ� ���¸� ���ÿ� �����ϴ� ���� ���ٰ� �����ϰ� �����Ѵ�.
			ChangeStatus(Move->BETarget, BEStatusId);
		}

		MoveEffectState = EMoveEffectState::ShowMoveEffect;
		Timer = MoveEffectShowTime;
	}
	else if (MoveEffectState == EMoveEffectState::ShowMoveEffect)
	{
		// TODO: Battle Effect �ִϸ��̼� ���

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
		// ù ƽ�� ��� ����, ���� ���� ������ ó���ϰ� �޽����� �غ��Ѵ�.
		const FPokemonMove* Move = Attacker->CurMove();
		EStatStageChangeType SEStatStageId = Move->SEStatStageId;
		EPokemonStatus SEStatusId = Move->SEStatusId;

		if (SEStatStageId != EStatStageChangeType::None)
		{
			ChangeStatStage(Move->SETarget, SEStatStageId, Move->SEStatStageValue);
		}
		else if (SEStatusId != EPokemonStatus::None)
		{
			// �ϴ� ���Ȱ� ���¸� ���ÿ� �����ϴ� ���� ���ٰ� �����ϰ� �����Ѵ�.
			ChangeStatus(Move->SETarget, SEStatusId);
		}

		MoveEffectState = EMoveEffectState::ShowMoveEffect;
		Timer = MoveEffectShowTime;
	}
	else if (MoveEffectState == EMoveEffectState::ShowMoveEffect)
	{
		// TODO: Secondary Effect �ִϸ��̼� ���

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
	// Leech Seed üũ
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
