#include "BattleStartStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"

ABattleStartStateMachine::ABattleStartStateMachine() 
{
}

ABattleStartStateMachine::~ABattleStartStateMachine() 
{
}

void ABattleStartStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleStartStateMachine::ESubstate::None:
		break;
	case ABattleStartStateMachine::ESubstate::FadeWait:
		ProcessFadeWait(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::GroundMove:
		ProcessGroundMove(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::ZClickWait:
		ProcessZClickWait(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::PlayerBattlerThrow:
		ProcessPlayerBattlerThrow(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove(_DeltaTime);
		break;
	case ABattleStartStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleStartStateMachine::ProcessFadeWait(float _DeltaTime)
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		State = ESubstate::GroundMove;
	}
}

void ABattleStartStateMachine::ProcessGroundMove(float _DeltaTime)
{
	Canvas->LerpShowGrounds(Timer / GroundMoveTime);

	if (Timer <= 0.0f)
	{
		Timer = EnemyPokemonBoxMoveTime;
		State = ESubstate::EnemyPokemonBoxMove;
	}
}

void ABattleStartStateMachine::ProcessEnemyPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::ZClickWait;
	}
}

void ABattleStartStateMachine::ProcessZClickWait(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		Timer = PlayerBattleThrowTime;
		State = ESubstate::PlayerBattlerThrow;
		Canvas->PlayBattlerThrowingAnimation();
		BallThrowAnimationPlayed = false;
	}
}

void ABattleStartStateMachine::ProcessPlayerBattlerThrow(float _DeltaTime)
{
	Canvas->LerpHidePlayerBattler(Timer / PlayerBattleThrowTime);

	if (Timer <= PlayerBattleThrowTime - BallThrowMotionWaitTime && false == BallThrowAnimationPlayed)
	{
		// 일정 시간이 지난 뒤 볼이 날아가는 애니메이션을 재생한다.
		Canvas->PlayThrowedBallAnimation();
		BallThrowAnimationPlayed = true;
	}

	if (true == BallThrowAnimationPlayed && Canvas->IsThrowedBallAnimationEnd())
	{
		Timer = PlayerPokemonTakeoutTime;
		Canvas->SetThrowedBallActive(false);
		State = ESubstate::PlayerPokemonTakeout;
	}
}

void ABattleStartStateMachine::ProcessPlayerPokemonTakeout(float _DeltaTime)
{
	Canvas->TakeOutPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		State = ESubstate::PlayerPokemonBoxMove;
	}
}

void ABattleStartStateMachine::ProcessPlayerPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowPlayerPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}