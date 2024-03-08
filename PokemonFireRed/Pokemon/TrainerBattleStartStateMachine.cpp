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
		ProcessEnemyBallMove();
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
		MsgBox->SetMessage(Enemy->GetTrainerNameW() + L"\nwould like to battle!");
		MsgBox->Write();
	}
}

void ATrainerBattleStartStateMachine::ProcessEnemyArrowMove()
{
	Canvas->LerpShowEnemyEntryArrow(Timer / EnemyArrowMoveTime);

	if (Timer <= 0)
	{
		
	}
}

void ATrainerBattleStartStateMachine::ProcessEnemyBallMove()
{
	if (Timer <= 0 && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		MsgBox->ShowSkipArrow();
	}
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
