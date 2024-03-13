#include "BattlePokeBallStateMachine.h"
#include "Battler.h"
#include "PokemonMsgBox.h"
#include "BattleCanvas.h"

ABattlePokeBallStateMachine::ABattlePokeBallStateMachine() 
{
}

ABattlePokeBallStateMachine::~ABattlePokeBallStateMachine() 
{
}

void ABattlePokeBallStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::BallUseMessage;
	MsgBox->SetMessage(L"RED used\nPOKé BALL!");
	MsgBox->Write();
}

bool ABattlePokeBallStateMachine::IsEnd() const
{
	return State == ESubstate::End;
}

void ABattlePokeBallStateMachine::Tick(float _DeltaTime)
{
	ABattleStateMachine::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::BallUseMessage:
		ProcessBallUseMessage();
		break;
	case ESubstate::PokeBallThrow:
		ProcessPokeBallThrow(_DeltaTime);
		break;
	case ESubstate::PokeBallBlocked:
		ProcessPokeBallBlocked();
		break;
	case ESubstate::DontBeAThiefMessage:
		ProcessDontBeAThiefMessage();
		break;
	case ESubstate::PokeBallPullInPokemon:
		ProcessPokeBallPullInPokemon();
		break;
	case ESubstate::PokeBallClosing:
		ProcessPokeBallClosing();
		break;
	case ESubstate::PokeBallDrop:
		ProcessPokeBallDrop(_DeltaTime);
		break;
	case ESubstate::PokeBallCheckBounceMore:
		ProcessPokeBallCheckBounceMore();
		break;
	case ESubstate::PokeBallBounce:
		ProcessPokeBallBounce(_DeltaTime);
		break;
	case ESubstate::TestCatch:
		ProcessTestCatch();
		break;
	case ESubstate::CatchResultAnim:
		ProcessCatchResultAnim();
		break;
	case ESubstate::CatchFailMessage:
		ProcessCatchFailMessage();
		break;
	case ESubstate::CatchSuccessMessage:
		ProcessCatchSuccessMessage();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}

}

void ABattlePokeBallStateMachine::ProcessBallUseMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::PokeBallThrow;

		Timer = BallThrowTime;
		Canvas->SetCatchBallActive(true);
		BallVelocity = ThrowVelocity;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallThrow(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, 350) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	if (Timer <= 0.0f)
	{
		if (true == Enemy->IsTrainer())
		{
			// Don't be a thief!
			// (트레이너에게 던지는 경우)
			// RED used\nPOKe BALL!
			// The TRAINER blocked the BALL!
			// 하고 플레이어 턴 종료됨

			//MsgBox->SetMessage(L"Ball missed!");
			//MsgBox->Write();
			State = ESubstate::End;
		}
		else
		{
			State = ESubstate::PokeBallPullInPokemon;
			Canvas->PlayCatchBallOpenAnimation();
			Timer = PullInTime;
		}
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlocked()
{
}

void ABattlePokeBallStateMachine::ProcessDontBeAThiefMessage()
{
}

void ABattlePokeBallStateMachine::ProcessPokeBallPullInPokemon()
{
	Canvas->LerpCatchPullInEnemyPokemon(Timer / PullInTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallClosing;
		Timer = ClosingTime;
		Canvas->PlayCatchBallCloseAnimation();
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallClosing()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::PokeBallDrop;
		BallVelocity = FVector::Zero;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallDrop(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, Gravity) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	// 땅 높이와 비슷해진 경우
	FVector BallPos = Canvas->GetCatchBallPosition();
	if (BallPos.Y >= BallGroundY)
	{
		State = ESubstate::PokeBallCheckBounceMore;
		BounceCount = MaxBounceCount;
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallCheckBounceMore()
{
	// 다 튕긴 경우
	if (BounceCount <= 0)
	{
		State = ESubstate::TestCatch;
		return;
	}

	State = ESubstate::PokeBallBounce;
	BallVelocity = -BallVelocity * Elasticity;
	Timer = BounceWaitTime;
}

void ABattlePokeBallStateMachine::ProcessPokeBallBounce(float _DeltaTime)
{
	BallVelocity += UPokemonUtil::PixelVector(0, Gravity) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallVelocity * _DeltaTime);

	// 일정 시간이 지난 뒤(튕기기 시작하는 시점에는 땅 높이와 비슷하기 때문) 땅 높이와 비슷해진 경우
	FVector BallPos = Canvas->GetCatchBallPosition();
	if (Timer <= 0.0f &&  BallPos.Y >= BallGroundY)
	{
		State = ESubstate::PokeBallCheckBounceMore;
		--BounceCount;
	}
}

void ABattlePokeBallStateMachine::ProcessTestCatch()
{
}

void ABattlePokeBallStateMachine::ProcessCatchResultAnim()
{
}

void ABattlePokeBallStateMachine::ProcessCatchFailMessage()
{
}

void ABattlePokeBallStateMachine::ProcessCatchSuccessMessage()
{
	//Gotcha RATTATA\nwas caught!
}

