#include "WildBattleStartStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "SoundManager.h"

AWildBattleStartStateMachine::AWildBattleStartStateMachine() 
{
}

AWildBattleStartStateMachine::~AWildBattleStartStateMachine() 
{
}

void AWildBattleStartStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::FadeWait;
	Timer = FadeWaitTime;
}

void AWildBattleStartStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case AWildBattleStartStateMachine::ESubstate::None:
		break;
	case AWildBattleStartStateMachine::ESubstate::FadeWait:
		ProcessFadeWait();
		break;
	case AWildBattleStartStateMachine::ESubstate::GroundMove:
		ProcessGroundMove();
		break;
	case AWildBattleStartStateMachine::ESubstate::EnemyPokemonBoxMove:
		ProcessEnemyPokemonBoxMove();
		break;
	case AWildBattleStartStateMachine::ESubstate::ZClickWait:
		ProcessZClickWait();
		break;
	case AWildBattleStartStateMachine::ESubstate::PlayerBattlerThrow:
		ProcessPlayerBattlerThrow();
		break;
	case AWildBattleStartStateMachine::ESubstate::PlayerPokemonTakeout:
		ProcessPlayerPokemonTakeout();
		break;
	case AWildBattleStartStateMachine::ESubstate::PlayerPokemonBoxMove:
		ProcessPlayerPokemonBoxMove();
		break;
	case AWildBattleStartStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void AWildBattleStartStateMachine::ProcessFadeWait()
{
	if (Timer <= 0.0f)
	{
		Timer = GroundMoveTime;
		State = ESubstate::GroundMove;
	}
}

void AWildBattleStartStateMachine::ProcessGroundMove()
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

void AWildBattleStartStateMachine::ProcessEnemyPokemonBoxMove()
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ZClickWait;
		MsgBox->ShowSkipArrow();
	}
}

void AWildBattleStartStateMachine::ProcessZClickWait()
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		State = ESubstate::PlayerBattlerThrow;
		USoundManager::PlaySE(RN::SEClick);
		Timer = PlayerBattleThrowTime;
		Canvas->PlayBattlerThrowingAnimation();
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
		BallThrowAnimationPlayed = false;
	}
}

void AWildBattleStartStateMachine::ProcessPlayerBattlerThrow()
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

void AWildBattleStartStateMachine::ProcessPlayerPokemonTakeout()
{
	Canvas->TakeOutPlayerPokemonFromBall(Timer / PlayerPokemonTakeoutTime);

	if (Timer <= 0.0f)
	{
		Timer = PlayerPokemonBoxMoveTime;
		State = ESubstate::PlayerPokemonBoxMove;
	}
}

void AWildBattleStartStateMachine::ProcessPlayerPokemonBoxMove()
{
	Canvas->LerpShowPlayerPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}