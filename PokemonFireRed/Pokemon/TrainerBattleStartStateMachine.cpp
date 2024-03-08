#include "TrainerBattleStartStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ATrainerBattleStartStateMachine::ATrainerBattleStartStateMachine() 
{
}

ATrainerBattleStartStateMachine::~ATrainerBattleStartStateMachine() 
{
}

void ATrainerBattleStartStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ATrainerBattleStartStateMachine::ESubstate::None:
		break;
	case ATrainerBattleStartStateMachine::ESubstate::FadeWait:
		ProcessFadeWait();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::GroundMove:
		ProcessGroundMove();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::EntryArrowMove:
		ProcessEnemyArrowMove();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::EntryBallMove:
		ProcessEnemyBallMove(_DeltaTime);
		break;
	case ATrainerBattleStartStateMachine::ESubstate::EnemyPokemonAppear:
		ProcessEnemyPokemonAppear();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::PlayerBattlerThrow:
		ProcessPlayerBattlerThrow();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove();
		break;
	case ATrainerBattleStartStateMachine::ESubstate::End:
		break;
	default:
		break;
	}

}

void ATrainerBattleStartStateMachine::ProcessFadeWait()
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		State = ESubstate::GroundMove;
	}
}

void ATrainerBattleStartStateMachine::ProcessGroundMove()
{
	Canvas->LerpShowGrounds(Timer / GroundMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::EntryArrowMove;
		Timer = EnemyArrowMoveTime;
	}
}

void ATrainerBattleStartStateMachine::ProcessEnemyArrowMove()
{
	Canvas->LerpShowEnemyEntryArrow(Timer / EnemyArrowMoveTime);

	if (Timer <= 0)
	{
		State = ESubstate::EntryBallMove;
		MsgBox->SetMessage(Enemy->GetTrainerNameW() + L"\nwould like to battle!");
		MsgBox->Write();
		MsgBox->SetWriteSpeed(0.75f);

		BallTimers.clear();
		BallTimers.resize(6);
		for (int i = 0; i < 6; ++i)
		{
			BallTimers[i] = EntryBallMoveTime;
		}
		MovingBallIndex = 0;

		Timer = EntryBallMoveInterval;
	}
}

void ATrainerBattleStartStateMachine::ProcessEnemyBallMove(float _DeltaTime)
{
	for (int i = 0; i <= MovingBallIndex; ++i)
	{
		BallTimers[i] -= _DeltaTime;
		Canvas->LerpEnemyEntryBall(i, BallTimers[i] / EntryBallMoveTime);
	}
	
	if (Timer <= 0.0f && MovingBallIndex < 5)
	{
		++MovingBallIndex;
		Timer = EntryBallMoveInterval;
	}
		
	if (BallTimers[5] < 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		MsgBox->ShowSkipArrow();
		MsgBox->SetWriteSpeed(1.0f);
		State = ESubstate::EnemyPokemonAppear;
	}
}

bool ATrainerBattleStartStateMachine::IsAllBallMoved()
{
	for (int i = 0; i < 6; ++i)
	{
		if (BallTimers[i] > 0.0f)
		{
			return false;
		}
	}
	return true;
}

void ATrainerBattleStartStateMachine::ProcessEnemyPokemonAppear()
{
}

void ATrainerBattleStartStateMachine::ProcessEnemyPokemonBoxMove()
{
}

void ATrainerBattleStartStateMachine::ProcessPlayerBattlerThrow()
{
}

void ATrainerBattleStartStateMachine::ProcessPlayerPokemonTakeout()
{
}

void ATrainerBattleStartStateMachine::ProcessPlayerPokemonBoxMove()
{
}
