#include "TrainerBattleStartStateMachine.h"

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

}

void ATrainerBattleStartStateMachine::ProcessEnemyArrowMove()
{
}

void ATrainerBattleStartStateMachine::ProcessEnemyBallMove()
{
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
