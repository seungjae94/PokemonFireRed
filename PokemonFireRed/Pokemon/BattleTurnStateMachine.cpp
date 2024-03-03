#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(
	ABattleCanvas* _Canvas,
	UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon,
	UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage,
	EBattleAction _PlayerAction, int _PlayerMoveIndex
)
{
	// �Է� ������ ����
	Canvas = _Canvas;
	PlayerPokemon = _PlayerPokemon;
	EnemyPokemon = _EnemyPokemon;
	PlayerStatStage = &_PlayerStatStage;
	EnemyStatStage = &_EnemyStatStage;
	PlayerAction = _PlayerAction;
	PlayerMoveId = PlayerPokemon->GetMoveId(_PlayerMoveIndex);

	// �� �׼� ����
	GenerateEnemyAction();

	// �� ���� ����
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(PlayerAction, EnemyAction, PlayerPokemon, EnemyPokemon, PlayerMoveId, EnemyMoveId, *PlayerStatStage, *EnemyStatStage);
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
	case ABattleTurnStateMachine::ESubstate::MoveSecondaryEffect:
		ProcessMoveSecondaryEffect(_DeltaTime);
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

void ABattleTurnStateMachine::GenerateEnemyAction()
{
	// ��� �ൿ ����
	EnemyAction = UBattleEnemyActionGenerator::Generate(EEnemyType::Wild, EnemyPokemon);
	int EnemyMoveIndex = UBattleEnemyActionGenerator::GetGeneratedMoveIndex();
	EnemyMoveId = EnemyPokemon->GetMoveId(EnemyMoveIndex);
}

void ABattleTurnStateMachine::DispatchTurn()
{
	EBattleAction AttackerAction;

	if (Attacker == PlayerPokemon)
	{
		AttackerAction = PlayerAction;
	}
	else
	{
		AttackerAction = EnemyAction;
	}

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
	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender, *AttackerStatStage, *DefenderStatStage, AttackMoveId);

	if (false == AccuracyCheckResult)
	{
		Canvas->SetBattleMessage(Attacker->GetNameW() + L"'s\nattack failed!");
		State = ESubstate::MoveFail;
		Timer = BattleMsgShowTime;
		return;
	}

	State = ESubstate::MoveAnim;
	const FPokemonMove* Move = UPokemonDB::FindMove(AttackMoveId);
	Canvas->SetBattleMessage(Attacker->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	Result = UDamageCalculator::CalcDamage(Attacker, Defender, *AttackerStatStage, *DefenderStatStage, AttackMoveId);
}

void ABattleTurnStateMachine::DispatchNextPhase()
{
	if (true == IsFirstTurn)
	{
		if (Attacker == PlayerPokemon)
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
	const FPokemonMove* Move = UPokemonDB::FindMove(AttackMoveId);
	ESecondaryEffectTarget SETarget = Move->SETarget;

	// �ΰ� ȿ���� ���� ��� �ٷ� ���� ������ �Ѿ��.
	if (SETarget == ESecondaryEffectTarget::None)
	{
		DispatchNextPhase();
		return;
	}

	// ���� �׽�Ʈ ������ �ΰ� ȿ���� �����Ѵ�.
	int RandomNumber = UPokemonMath::RandomInt(0, 99);
	if (RandomNumber < Move->SERate)
	{
		State = ESubstate::MoveSecondaryEffect;
		SEState = ESecondaryEffectState::None;
		return;
	}

	DispatchNextPhase();
}

void ABattleTurnStateMachine::DispatchEndOfTurn()
{
	// TODO: ���� EOT�� ���� �������� ����
	State = ESubstate::EndOfTurn;
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
			if (Attacker == PlayerPokemon)
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
			// (��Ģ�����δ�) End Of Turn�� ���� ������ ���� ����� ����� �Ѵ�.
			// ������ ���ݱ��� ������ ���븸 ����ϸ� ��� ��� ������ EOT ������ ����.
			IsPlayerFirstEOT = IsPlayerFirst; // UTurnOrderCalculator::IsPlayerFirstEOT(..., IsPlayerFirst);

			if (true == IsPlayerFirstEOT)
			{
				SetPlayerAsAttacker();
			}
			else
			{
				SetEnemyAsAttacker();
			}

			DispatchEndOfTurn();
			return;
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveAnim(float _DeltaTime)
{
	// �ִϸ��̼�: ���� ����
	State = ESubstate::MoveDamage;
	Timer = DamageTime;
	MoveResultMsg = EMoveResultMsg::None;
	PrevHp = Defender->GetCurHp();
	NextHp = UPokemonMath::Max(PrevHp - Result.Damage, 0);
}

void ABattleTurnStateMachine::ProcessMoveDamage(float _DeltaTime)
{
	if (MoveResultMsg == EMoveResultMsg::None)
	{
		if (Defender == PlayerPokemon)
		{
			Canvas->LerpPlayerHpInfo(PrevHp, NextHp, Defender->GetHp(), Timer / DamageTime);
		}
		else
		{
			Canvas->LerpEnemyHpInfo(PrevHp, NextHp, Defender->GetHp(), Timer / DamageTime);
		}

		if (Timer <= 0.0f)
		{
			// ���� ������ ó��
			Defender->SetCurHp(NextHp);
			
			// ������ ��� ��� ������
			if (Defender == PlayerPokemon)
			{
				Canvas->RefreshPlayerPokemonCurHpText();
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
				Canvas->SetBattleMessage(Result.GetTypeVsW(Defender->GetNameW()));
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
				Canvas->SetBattleMessage(Result.GetTypeVsW(Defender->GetNameW()));
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

void ABattleTurnStateMachine::ProcessMoveSecondaryEffect(float _DeltaTime)
{
	if (SEState == ESecondaryEffectState::None)
	{
		// ù ƽ�� ��� ����, ���� ���� ������ ó���ϰ� �޽����� �غ��Ѵ�.
		const FPokemonMove* Move = UPokemonDB::FindMove(AttackMoveId);
		ESecondaryEffectStatStage SEStatStageId = Move->SEStatStageId;
		EPokemonStatus SEStatusId = Move->SEStatusId;
		if (SEStatStageId != ESecondaryEffectStatStage::None)
		{
			// ���� ���/�ϰ� ����

			// ���� ���/�ϰ� �޽���
			SEMessage = L"Debug - Stat Changed.";
		}
		else if (SEStatusId != EPokemonStatus::None)
		{
			// ���� ���� ����

			// ���� ���� �޽���
			SEMessage = L"Debug - Status Changed.";
		}

		SEState = ESecondaryEffectState::StatStageEffect;
		Timer = SEEffectShowTime;
	}
	else if (SEState == ESecondaryEffectState::StatStageEffect)
	{
		// TODO: Secondary Effect �ִϸ��̼� ���

		if (Timer <= 0.0f)
		{
			SEState = ESecondaryEffectState::ShowSEMessage;
			Timer = BattleMsgShowTime;
			Canvas->SetBattleMessage(SEMessage);
		}
	}
	else /*SEState == ESecondaryEffectState::ShowSEMessage*/
	{
		if (Timer <= 0.0f)
		{
			DispatchNextPhase();
		}
	}
}


void ABattleTurnStateMachine::ProcessEndOfTurn(float _DeltaTime)
{
	// TODO
	State = ESubstate::End;
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = PlayerPokemon;
	Defender = EnemyPokemon;
	AttackerStatStage = PlayerStatStage;
	DefenderStatStage = EnemyStatStage;
	AttackMoveId = PlayerMoveId;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = EnemyPokemon;
	Defender = PlayerPokemon;
	AttackerStatStage = EnemyStatStage;
	DefenderStatStage = PlayerStatStage;
	AttackMoveId = EnemyMoveId;
}