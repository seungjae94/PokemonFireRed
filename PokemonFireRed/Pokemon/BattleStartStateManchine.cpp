#include "BattleStartStateManchine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"

ABattleStartStateManchine::ABattleStartStateManchine() 
{
}

ABattleStartStateManchine::~ABattleStartStateManchine() 
{
}

void ABattleStartStateManchine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleStartStateManchine::ESubstate::None:
		break;
	case ABattleStartStateManchine::ESubstate::FadeWait:
		ProcessFadeWait(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::GroundMove:
		ProcessGroundMove(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::ZClickWait:
		ProcessZClickWait(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::PlayerBattlerThrow:
		ProcessPlayerBattlerThrow(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove(_DeltaTime);
		break;
	case ABattleStartStateManchine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleStartStateManchine::ProcessFadeWait(float _DeltaTime)
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		State = ESubstate::GroundMove;
	}
}

void ABattleStartStateManchine::ProcessGroundMove(float _DeltaTime)
{
	Canvas->LerpShowGrounds(Timer / GroundMoveTime);

	if (Timer <= 0.0f)
	{
		Timer = EnemyPokemonBoxMoveTime;
		State = ESubstate::EnemyPokemonBoxMove;
	}
}

void ABattleStartStateManchine::ProcessEnemyPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::ZClickWait;
	}
}

void ABattleStartStateManchine::ProcessZClickWait(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		Timer = PlayerBattleThrowTime;
		State = ESubstate::PlayerBattlerThrow;
		Canvas->PlayBattlerThrowingAnimation();
		BallThrowAnimationPlayed = false;
	}
}

void ABattleStartStateManchine::ProcessPlayerBattlerThrow(float _DeltaTime)
{
	Canvas->LerpHidePlayerBattler(Timer / PlayerBattleThrowTime);

	if (Timer <= PlayerBattleThrowTime - BallThrowMotionWaitTime && false == BallThrowAnimationPlayed)
	{
		// ���� �ð��� ���� �� ���� ���ư��� �ִϸ��̼��� ����Ѵ�.
		Canvas->PlayThrowedBallAnimation();
		BallThrowAnimationPlayed = true;
	}

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonTakeoutTime;
		State = ESubstate::PlayerPokemonTakeout;
	}
}

void ABattleStartStateManchine::ProcessPlayerPokemonTakeout(float _DeltaTime)
{
	Canvas->TakeOutPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		State = ESubstate::PlayerPokemonBoxMove;
	}
}

void ABattleStartStateManchine::ProcessPlayerPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowPlayerPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}