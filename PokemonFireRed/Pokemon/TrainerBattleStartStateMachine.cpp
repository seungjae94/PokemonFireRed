#include "TrainerBattleStartStateMachine.h"
#include <EnginePlatform/EngineInput.h>
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
	case ESubstate::None:
		break;
	case ESubstate::FadeWait:
		ProcessFadeWait();
		break;
	case ESubstate::GroundMove:
		ProcessGroundMove();
		break;
	case ESubstate::EntryArrowMove:
		ProcessEnemyArrowMove();
		break;
	case ESubstate::EntryBallMove:
		ProcessEnemyBallMove(_DeltaTime);
		break;
	case ESubstate::ZClickWait:
		ProcessZClickWait();
		break;
	case ESubstate::EnemyPokemonAppear:
		ProcessEnemyPokemonAppear(_DeltaTime);
		break;
	case ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove();
		break;
	case ESubstate::PlayerBattlerThrow1:
		ProcessPlayerBattlerThrow1();
		break;
	case ESubstate::PlayerBattlerThrow2:
		ProcessPlayerBattlerThrow2();
		break;
	case ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout();
		break;
	case ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove();
		break;
	case ESubstate::End:
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
		State = ESubstate::ZClickWait;
	}
}

void ATrainerBattleStartStateMachine::ProcessZClickWait()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::EnemyPokemonAppear;
		MsgBox->SetMessage(Enemy->GetTrainerNameW() + L" sent\nout " + Enemy->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
		
		Canvas->PlayEnemyGroundBallAnimation();

		EntryFadeTimer = EntryFadeTime;
		EnemyPokemonAppearTimer = EnemyPokemonAppearTime;
	}
}

void ATrainerBattleStartStateMachine::ProcessEnemyPokemonAppear(float _DeltaTime)
{
	EntryFadeTimer -= _DeltaTime;
	EnemyPokemonAppearTimer -= _DeltaTime;

	if (true == Canvas->IsEnemyGroundBallAnimationEnd())
	{
		Canvas->SetEnemyGroundBallActive(false);
	}

	Canvas->LerpHideEnemyEntry(EntryFadeTimer / EntryFadeTime);
	Canvas->LerpHideEnemyBattler(EntryFadeTimer / EntryFadeTime);
	Canvas->TakeOutEnemyPokemonFromBall(EnemyPokemonAppearTimer / EnemyPokemonAppearTime);

	if (EntryFadeTimer <= 0.0f 
		&& EnemyPokemonAppearTimer <= 0.0f 
		&& MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::EnemyPokemonBoxMove;
		Timer = EnemyPokemonBoxMoveTime;
	}

}

void ATrainerBattleStartStateMachine::ProcessEnemyPokemonBoxMove()
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PlayerBattlerThrow1;
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();

		Canvas->PlayBattlerThrowingAnimation();
		Timer = PlayerBattlerHideTime;
	}
}

void ATrainerBattleStartStateMachine::ProcessPlayerBattlerThrow1()
{
	Canvas->LerpHidePlayerBattler(Timer / PlayerBattlerHideTime);

	if (PlayerBattlerHideTime - Timer >= 0.5f)
	{
		Canvas->PlayThrowedBallAnimation();
		State = ESubstate::PlayerBattlerThrow2;
	}
}

void ATrainerBattleStartStateMachine::ProcessPlayerBattlerThrow2()
{
	Canvas->LerpHidePlayerBattler(Timer / PlayerBattlerHideTime);

	if (Timer <= 0.0f && true == Canvas->IsThrowedBallAnimationEnd())
	{
		State = ESubstate::PlayerPokemonTakeout;
		Canvas->SetThrowedBallActive(false);
		Timer = PlayerPokemonTakeoutTime;
	}
}

void ATrainerBattleStartStateMachine::ProcessPlayerPokemonTakeout()
{
	Canvas->TakeOutPlayerPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PlayerPokemonBoxMove;
		Timer = PlayerPokemonBoxMoveTime;
	}
}

void ATrainerBattleStartStateMachine::ProcessPlayerPokemonBoxMove()
{
	Canvas->LerpShowPlayerPokemonBox(Timer / PlayerPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}

}
