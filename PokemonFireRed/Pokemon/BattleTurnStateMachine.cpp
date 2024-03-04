#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"
#include "Battler.h"

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
	const UPokemon* AttackerPokemon = Attacker->CurPokemonReadonly();

	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender);

	if (false == AccuracyCheckResult)
	{
		if (Move->BETarget == EMoveEffectTarget::Self)
		{
			Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L"'s\nattack failed!");
		}
		else
		{
			Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L"'s\nattack missed!");
		}

		State = ESubstate::MoveFail;
		Timer = BattleMsgShowTime;
		return;
	}

	State = ESubstate::MoveAnim;
	Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	Result = UDamageCalculator::CalcDamage(Attacker, Defender);
}

void ABattleTurnStateMachine::DispatchNextPhase()
{
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
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::DispatchSecondaryEffect()
{
	const FPokemonMove* Move = Attacker->CurMove();
	EMoveEffectTarget SETarget = Move->SETarget;

	// SE ����� �ڱ� �ڽ��� ��� SE�� �õ��Ѵ�.
	if (SETarget == EMoveEffectTarget::Self)
	{
		TrySecondaryEffect();
		return;
	}

	// Defender�� ������ ��� SE�� ��ŵ�ϰ� Faint ó���� �Ѵ�.
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetCurHp() == 0)
	{
		State = ESubstate::Faint;
		return;
	}

	// SE�� ���� ��� �ٷ� ���� ������ �Ѿ��.
	if (SETarget == EMoveEffectTarget::None)
	{
		DispatchNextPhase();
		return;
	}

	// SE�� �ִ� ��� SE�� �õ��Ѵ�.
	TrySecondaryEffect();
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
		if (true == IsFirstTurn)
		{
			IsFirstTurn = false;
			if (Attacker == Player)
			{
				SetEnemyAsAttacker();
				DispatchTurn();
			}
			else
			{
				SetPlayerAsAttacker();
				DispatchTurn();
			}
			return;
		}
		else
		{
			DispatchEndOfTurn();
			return;
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveAnim(float _DeltaTime)
{
	// �ִϸ��̼�: ���� ����

	// �ִϸ��̼� ����
	const FPokemonMove* Move = Attacker->CurMove();

	if (Move->BETarget == EMoveEffectTarget::None)
	{
		// ���� ����� ���
		State = ESubstate::MoveDamage;
		Timer = DamageTime;
		MoveResultMsg = EMoveResultMsg::None;
		PrevHp = Defender->CurPokemonReadonly()->GetCurHp();
		NextHp = UPokemonMath::Max(PrevHp - Result.Damage, 0);
	}
	else
	{
		State = ESubstate::MoveBattleEffect;
		MoveEffectState = EMoveEffectState::None;
	}
}

void ABattleTurnStateMachine::ProcessMoveDamage(float _DeltaTime)
{
	UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (MoveResultMsg == EMoveResultMsg::None)
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
				MoveResultMsg = EMoveResultMsg::Critical;
				Canvas->SetBattleMessage(L"A critical hit!");
				Timer = BattleMsgShowTime;
				return;
			}
			else if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// ������ ����
			DispatchSecondaryEffect();
		}
	}
	else if (MoveResultMsg == EMoveResultMsg::Critical)
	{
		if (Timer <= 0.0f)
		{
			if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// ������ ����
			DispatchSecondaryEffect();
		}
	}
	else /*MoveResultMsg == EMoveResultMsg::TypeEffect*/
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
			ChangeStatus();
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
			ChangeStatus();
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
			Canvas->SetBattleMessage(Defender->CurPokemonReadonly()->GetNameW() + L"\nfainted!");
		}
	}
	else if (FaintState == EFaintState::HidePokemon && Defender == Enemy)
	{
		Canvas->LerpFaintEnemyPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			FaintState = EFaintState::ShowFaintMessage;
			Canvas->SetEnemyPokemonBoxActive(false);

			if (Defender->IsWildPokemon())
			{
				Canvas->SetBattleMessage(L"Wild " + Defender->CurPokemonReadonly()->GetNameW() + L"\nfainted!");
			}
			else
			{
				Canvas->SetBattleMessage(L"Foe " + Defender->CurPokemonReadonly()->GetNameW() + L"\nfainted!");
			}
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

	MoveEffectMessage = L"";
	if (Target == Enemy)
	{
		MoveEffectMessage += L"Foe ";
	}

	UPokemon* TargetPokemon = Target->CurPokemon();

	MoveEffectMessage += TargetPokemon->GetNameW();
	MoveEffectMessage += L"'s ";

	int Value = _MEStatStageValue;
	std::wstring Suffix = GetStatStageMessageSuffix(Value);
	switch (_MEStatStageId)
	{
	case EStatStageChangeType::Atk:
		MoveEffectMessage += L"ATTACK\n";
		Target->StatStage.AddAtk(Value);
		break;
	case EStatStageChangeType::Def:
		MoveEffectMessage += L"DEFENSE\n";
		Target->StatStage.AddDef(Value);
		break;
	case EStatStageChangeType::SpAtk:
		MoveEffectMessage += L"SP.ATK\n";
		Target->StatStage.AddSpAtk(Value);
		break;
	case EStatStageChangeType::SpDef:
		MoveEffectMessage += L"SP.DEF\n";
		Target->StatStage.AddDef(Value);
		break;
	case EStatStageChangeType::Speed:
		MoveEffectMessage += L"SPEED\n";
		Target->StatStage.AddSpeed(Value);
		break;
	case EStatStageChangeType::Accuracy:
		MoveEffectMessage += L"accuracy\n";
		Target->StatStage.AddAccuracy(Value);
		break;
	case EStatStageChangeType::Evasion:
		MoveEffectMessage += L"evasiveness\n";
		Target->StatStage.AddEvasion(Value);
		break;
	default:
		break;
	}
	MoveEffectMessage += Suffix;
}

void ABattleTurnStateMachine::ChangeStatus()
{
}

std::wstring ABattleTurnStateMachine::GetStatStageMessageSuffix(int _Value)
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

void ABattleTurnStateMachine::TrySecondaryEffect()
{
	const FPokemonMove* Move = Attacker->CurMove();

	// ���� �׽�Ʈ ������ �ΰ� ȿ���� �����Ѵ�.
	int RandomNumber = UPokemonMath::RandomInt(0, 99);
	if (RandomNumber < Move->SERate)
	{
		State = ESubstate::MoveSecondaryEffect;
		MoveEffectState = EMoveEffectState::None;
		return;
	}

	// ���� �׽�Ʈ ���н� ���� ������� �����Ѵ�.
	DispatchNextPhase();
}
