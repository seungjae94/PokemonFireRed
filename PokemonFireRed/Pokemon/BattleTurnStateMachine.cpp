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

void ABattleTurnStateMachine::ProcessAction1()
{
	if (true == BattleActionSM->IsEnd())
	{
		State = ESubstate::Action1Faint;
		BattleFaintSM->Start(Canvas, Attacker, Defender);
	}
}

void ABattleTurnStateMachine::ProcessAction1Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� ��� ��Ʋ�� �����ؾ� �Ѵ�.
		// ��� ��Ʋ�� �����ؾ� �ϱ� ������ �ϵ� ��� �����Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// ���� ��� ���������� Action2�� ������ �� �ִ����� Ȯ���� ����� �Ѵ�.
		State = ESubstate::TestAction2;
		SwapAttackerAndDefender();
	}
}

void ABattleTurnStateMachine::ProcessTestAction2()
{
	// Attacker�� Defender�� Faint ���¶�� Action2�� ������ �� ����.
	if (true == Attacker->CurPokemon()->IsFaint() || true == Defender->CurPokemon()->IsFaint())
	{
		State = ESubstate::StartEndOfTurn;
		return;
	}

	// �� ���� ��� Action2�� �����Ѵ�.
	State = ESubstate::Action2;
	BattleActionSM->Start(Canvas, Attacker, Defender);
}

void ABattleTurnStateMachine::ProcessAction2()
{
	if (true == BattleActionSM->IsEnd())
	{
		State = ESubstate::Action2Faint;
		BattleFaintSM->Start(Canvas, Attacker, Defender);
	}
}

void ABattleTurnStateMachine::ProcessAction2Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == Attacker->AllFaint() || true == Defender->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// EOT �ܰ�� �Ѿ��.
		State = ESubstate::StartEndOfTurn;
	}
}

void ABattleTurnStateMachine::ProcessStartEndOfTurn()
{
	// EOT ������ �����Ѵ�.
	bool IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);

	if (true == IsPlayerFirst)
	{
		SetPlayerAsEOTTarget();
	}
	else
	{
		SetEnemyAsEOTTarget();
	}

	// EOT1�� ������ �� �ִ��� üũ�Ѵ�.
	State = ESubstate::TestEndOfTurn1;
	BattleEOTSM->Start(Canvas, EOTTarget, EOTNextTarget);
}

void ABattleTurnStateMachine::ProcessTestEndOfTurn1()
{
	// EOTTarget�� Faint ���¶�� EOT1�� ������ �� ����.
	if (true == EOTTarget->CurPokemon()->IsFaint())
	{
		State = ESubstate::TestEndOfTurn2;
		return;
	}

	// �� ���� ��� EOT1�� �����Ѵ�.
	State = ESubstate::EndOfTurn1;
	BattleEOTSM->Start(Canvas, EOTTarget, EOTNextTarget);
}

void ABattleTurnStateMachine::ProcessEndOfTurn1()
{
	if (true == BattleEOTSM->IsEnd())
	{
		State = ESubstate::EndOfTurn1Faint;
		BattleFaintSM->Start(Canvas, EOTNextTarget, EOTTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn1Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� ��� �����ؾ� �Ѵ�.
		if (true == EOTTarget->AllFaint() || true == EOTNextTarget->AllFaint())
		{
			EndTurnWithReason();
			return;
		}

		// EOT2�� ������ �� �ִ��� üũ�Ѵ�.
		State = ESubstate::TestEndOfTurn2;
	}
}

void ABattleTurnStateMachine::ProcessTestEndOfTurn2()
{
	// EOTNextTarget�� Faint ���¶�� EOT2�� ������ �� ����.
	if (true == EOTNextTarget->CurPokemon()->IsFaint())
	{
		State = ESubstate::End;
		return;
	}

	// �� ���� ��� EOT2�� �����Ѵ�.
	State = ESubstate::EndOfTurn2;
	BattleEOTSM->Start(Canvas, EOTNextTarget, EOTTarget);
}

void ABattleTurnStateMachine::ProcessEndOfTurn2()
{
	if (true == BattleEOTSM->IsEnd())
	{
		State = ESubstate::EndOfTurn2Faint;
		BattleFaintSM->Start(Canvas, EOTTarget, EOTNextTarget);
	}
}

void ABattleTurnStateMachine::ProcessEndOfTurn2Faint()
{
	if (true == BattleFaintSM->IsEnd())
	{
		// ��� �� ���� ���� ���� ��� �� ���� ������ ����ϰ� �����ؾ� �Ѵ�.
		if (true == EOTTarget->AllFaint() || true == EOTNextTarget->AllFaint())
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


void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::Action1:
		ProcessAction1();
		break;
	case ABattleTurnStateMachine::ESubstate::Action1Faint:
		ProcessAction1Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::TestAction2:
		ProcessTestAction2();
		break;
	case ABattleTurnStateMachine::ESubstate::Action2:
		ProcessAction2();
		break;
	case ABattleTurnStateMachine::ESubstate::Action2Faint:
		ProcessAction2Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::StartEndOfTurn:
		ProcessStartEndOfTurn();
		break;
	case ABattleTurnStateMachine::ESubstate::TestEndOfTurn1:
		ProcessTestEndOfTurn1();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn1:
		ProcessEndOfTurn1();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn1Faint:
		ProcessEndOfTurn1Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::TestEndOfTurn2:
		ProcessTestEndOfTurn2();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn2:
		ProcessEndOfTurn2();
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn2Faint:
		ProcessEndOfTurn2Faint();
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}