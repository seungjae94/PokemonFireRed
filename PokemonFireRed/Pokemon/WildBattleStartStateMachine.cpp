#include "WildBattleStartStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

AWildWildBattleStartStateMachine::AWildWildBattleStartStateMachine() 
{
}

AWildWildBattleStartStateMachine::~AWildWildBattleStartStateMachine() 
{
}

void AWildWildBattleStartStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case AWildWildBattleStartStateMachine::ESubstate::None:
		break;
	case AWildWildBattleStartStateMachine::ESubstate::FadeWait:
		ProcessFadeWait(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::GroundMove:
		ProcessGroundMove(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::ZClickWait:
		ProcessZClickWait(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::PlayerBattlerThrow:
		ProcessPlayerBattlerThrow(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove(_DeltaTime);
		break;
	case AWildWildBattleStartStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void AWildWildBattleStartStateMachine::ProcessFadeWait(float _DeltaTime)
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		State = ESubstate::GroundMove;
	}
}

void AWildWildBattleStartStateMachine::ProcessGroundMove(float _DeltaTime)
{
	Canvas->LerpShowGrounds(Timer / GroundMoveTime);

	if (Timer <= 0.0f)
	{
		Timer = EnemyPokemonBoxMoveTime;

		MsgBox->SetMessage(L"Wild " + Enemy->CurPokemonReadonly()->GetNameW() + L" appeared!");
		MsgBox->Write();

		State = ESubstate::EnemyPokemonBoxMove;
	}
}

void AWildWildBattleStartStateMachine::ProcessEnemyPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ZClickWait;
		MsgBox->ShowSkipArrow();
	}
}

void AWildWildBattleStartStateMachine::ProcessZClickWait(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		Timer = PlayerBattleThrowTime;
		State = ESubstate::PlayerBattlerThrow;
		Canvas->PlayBattlerThrowingAnimation();
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
		BallThrowAnimationPlayed = false;
	}
}

void AWildWildBattleStartStateMachine::ProcessPlayerBattlerThrow(float _DeltaTime)
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

void AWildWildBattleStartStateMachine::ProcessPlayerPokemonTakeout(float _DeltaTime)
{
	Canvas->TakeOutPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		State = ESubstate::PlayerPokemonBoxMove;
	}
}

void AWildWildBattleStartStateMachine::ProcessPlayerPokemonBoxMove(float _DeltaTime)
{
	Canvas->LerpShowPlayerPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}