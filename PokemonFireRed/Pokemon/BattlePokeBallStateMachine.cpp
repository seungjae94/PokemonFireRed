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
	case ABattlePokeBallStateMachine::ESubstate::None:
		break;
	case ABattlePokeBallStateMachine::ESubstate::BallUseMessage:
		ProcessBallUseMessage();
		break;
	case ABattlePokeBallStateMachine::ESubstate::PokeBallThrow:
		ProcessPokeBallThrow(_DeltaTime);
		break;
	case ABattlePokeBallStateMachine::ESubstate::PokeBallBlocked:
		ProcessPokeBallBlocked();
		break;
	case ABattlePokeBallStateMachine::ESubstate::DontBeAThiefMessage:
		ProcessDontBeAThiefMessage();
		break;
	case ABattlePokeBallStateMachine::ESubstate::PokeBallVerticalMove:
		ProcessPokeBallVerticalMove();
		break;
	case ABattlePokeBallStateMachine::ESubstate::TestCatch:
		ProcessTestCatch();
		break;
	case ABattlePokeBallStateMachine::ESubstate::CatchResultAnim:
		ProcessCatchResultAnim();
		break;
	case ABattlePokeBallStateMachine::ESubstate::CatchFailMessage:
		ProcessCatchFailMessage();
		break;
	case ABattlePokeBallStateMachine::ESubstate::CatchSuccessMessage:
		ProcessCatchSuccessMessage();
		break;
	case ABattlePokeBallStateMachine::ESubstate::End:
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
		BallThrowVelocity = UPokemonUtil::PixelVector(200, -200);
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallThrow(float _DeltaTime)
{
	BallThrowVelocity += UPokemonUtil::PixelVector(0, 250) * _DeltaTime;
	Canvas->AddCatchBallPosition(BallThrowVelocity * _DeltaTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;


		if (true == Enemy->IsTrainer())
		{
			// Don't be a thief!
			// (트레이너에게 던지는 경우)
			// RED used\nPOKe BALL!
			// The TRAINER blocked the BALL!
			// 하고 플레이어 턴 종료됨

			//MsgBox->SetMessage(L"Ball missed!");
			//MsgBox->Write();
		}
		else
		{
			//Gotcha RATTATA\nwas caught!
		}
	}
}

void ABattlePokeBallStateMachine::ProcessPokeBallBlocked()
{
}

void ABattlePokeBallStateMachine::ProcessDontBeAThiefMessage()
{
}

void ABattlePokeBallStateMachine::ProcessPokeBallVerticalMove()
{
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
}

