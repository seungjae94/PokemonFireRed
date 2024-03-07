#include "BattlePlayerShiftStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ABattlePlayerShiftStateMachine::ABattlePlayerShiftStateMachine() 
{
}

ABattlePlayerShiftStateMachine::~ABattlePlayerShiftStateMachine() 
{
}

void ABattlePlayerShiftStateMachine::Start(std::wstring_view _TakeInPokemonName, ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Player)
{
	TakeInPokemonName = _TakeInPokemonName;
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Player = _Player;

	State = ESubstate::TakeIn;
	Timer = WaitTime;
	MsgBox->SetMessage(TakeInPokemonName + L", that's enough!\nCome back!");
	MsgBox->Write();
}

void ABattlePlayerShiftStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::Wait1:
		ProcessWait1();
		break;
	case ESubstate::TakeIn:
		ProcessTakeIn();
		break;
	case ESubstate::Wait2:
		ProcessWait2();
		break;
	case ESubstate::ThrowBall:
		ProcessThrowBall();
		break;
	case ESubstate::SendOut:
		ProcessSendOut();
		break;
	case ESubstate::EndWait:
		ProcessEndWait();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePlayerShiftStateMachine::ProcessWait1()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TakeIn;
		Timer = TakeInTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessTakeIn()
{
	Canvas->TakeInPokemonToBall(Timer / TakeInTime);

	if (Timer <= 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::Wait2;
		Timer = WaitTime;
		Canvas->SetPlayerPokemonBoxActive(false);
		Canvas->SetPlayerPokemonImageActive(false);
	}
}

void ABattlePlayerShiftStateMachine::ProcessWait2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::ThrowBall;
		Canvas->PlayThrowedBallAnimation();
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
	}
}

void ABattlePlayerShiftStateMachine::ProcessThrowBall()
{
	if (true == Canvas->IsThrowedBallAnimationEnd())
	{
		State = ESubstate::SendOut;
		Timer = SendOutTime;
		Canvas->SetThrowedBallActive(false);
		Canvas->SetPlayerPokemonBoxActive(true);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshMoveSelectBox();
		Canvas->SetPlayerPokemonImageActive(true);
	}
}

void ABattlePlayerShiftStateMachine::ProcessSendOut()
{
	Canvas->TakeOutPokemonFromBall(Timer / SendOutTime);

	if (Timer <= 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::EndWait;
		Timer = WaitTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessEndWait()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}
