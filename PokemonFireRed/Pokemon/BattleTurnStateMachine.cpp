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
	bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}

	State = ESubstate::Action1;
	Reason = EEndReason::None;
	BattleActionSM->Start(Canvas, Attacker, Defender);
}

void ABattleTurnStateMachine::ProcessAction1(float _DeltaTime)
{
	if (true == BattleActionSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// Attacker �Ǵ� Defender�� �׾� ������ EOT�� ������.
		const UPokemon* AttackerPokemon = Attacker->CurPokemon();
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		bool AttackerFaint = (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint);
		bool DefenderFaint = (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (AttackerFaint && DefenderFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::End;
			return;
		}
		else if (AttackerFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Defender);
			return;
		}
		else if (DefenderFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Attacker);
		}

		// �� �� ����ִٸ� Action2�� ������.
		State = ESubstate::Action2;

		SwapAttackerAndDefender();
		BattleActionSM->Start(Canvas, Attacker, Defender);
	}
}

void ABattleTurnStateMachine::ProcessAction2(float _DeltaTime)
{
	if (true == BattleActionSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// Attacker �Ǵ� Defender�� �׾� ������ EOT�� ������.
		const UPokemon* AttackerPokemon = Attacker->CurPokemon();
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		bool AttackerFaint = (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint);
		bool DefenderFaint = (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (AttackerFaint && DefenderFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::End;
			return;
		}
		else if (AttackerFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Defender);
			return;
		}
		else if (DefenderFaint)
		{
			// ����Ʈ ����Ʈ�� BattleActionSM�� ó���Ѵ�.
			State = ESubstate::EndOfTurn2;
			BattleEOTSM->Start(Canvas, Attacker);
		}

		// �� �� ����ִٸ� EOT1�� ������.
		bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);

		if (true == IsPlayerFirst)
		{
			SetPlayerAsEOTTarget();
		}
		else
		{
			SetEnemyAsEOTTarget();
		}

		State = ESubstate::EndOfTurn1;
		BattleEOTSM->Start(Canvas, EOTTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn1(float _DeltaTime)
{
	if (true == BattleEOTSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		const UPokemon* NextPokemon = EOTNextTarget->CurPokemon();

		bool NextPokemonFaint = (NextPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (true == NextPokemonFaint)
		{
			// ���� ���ϸ��� �׾� EOT�� ������� ���� ��� �ٷ� ���� �����Ѵ�.
			State = ESubstate::End;
			return;
		}

		State = ESubstate::EndOfTurn2;
		BattleEOTSM->Start(Canvas, EOTNextTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn2(float _DeltaTime)
{
	if (true == BattleEOTSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		State = ESubstate::End;
	}
}

void ABattleTurnStateMachine::SwapAttackerAndDefender()
{
	UBattler* Tmp = Attacker;
	Attacker = Defender;
	Defender = Tmp;
}

void ABattleTurnStateMachine::EndTurnWithReason()
{
	if (true == Enemy->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EEndReason::WinToWild;
		}
		else
		{
			Reason = EEndReason::WinToTrainer;
		}
	}
	else if (true == Player->AllFaint())
	{
		if (true == Enemy->IsWildPokemon())
		{
			Reason = EEndReason::LoseToWild;
		}
		else
		{
			Reason = EEndReason::LoseToTrainer;
		}
	}

	State = ESubstate::End;
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::Action1:
		ProcessAction1(_DeltaTime);
		break;
	case ESubstate::Action2:
		ProcessAction2(_DeltaTime);
		break;
	case ESubstate::EndOfTurn1:
		ProcessEndOfTurn1(_DeltaTime);
		break;
	case ESubstate::EndOfTurn2:
		ProcessEndOfTurn2(_DeltaTime);
		break;
	default:
		break;
	}
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

void ABattleTurnStateMachine::SetPlayerAsEOTTarget()
{
	EOTTarget = Player;
	EOTNextTarget = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsEOTTarget()
{
	EOTTarget = Enemy;
	EOTNextTarget = Player;
}