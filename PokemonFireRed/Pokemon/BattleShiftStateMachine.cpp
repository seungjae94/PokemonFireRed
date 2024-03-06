#include "BattleShiftStateMachine.h"

ABattleShiftStateMachine::ABattleShiftStateMachine() 
{
}

ABattleShiftStateMachine::~ABattleShiftStateMachine() 
{
}

void ABattleShiftStateMachine::Start(std::wstring_view _TakeInPokemonName, ABattleCanvas* _Canvas, const UBattler* _Player)
{
	TakeInPokemonName = _TakeInPokemonName;
	Canvas = _Canvas;
	Player = _Player;

	State = ESubstate::TakeIn;
	Timer = WaitTime;
	Canvas->SetBattleMessage(TakeInPokemonName + L", that's enough!\nCome back!");
}

void ABattleShiftStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleShiftStateMachine::ESubstate::None:
		break;
	case ESubstate::Wait1:
		ProcessWait1();
		break;
	case ABattleShiftStateMachine::ESubstate::TakeIn:
		ProcessTakeIn();
		break;
	case ESubstate::Wait2:
		ProcessWait2();
		break;
	case ABattleShiftStateMachine::ESubstate::ThrowBall:
		ProcessThrowBall();
		break;
	case ABattleShiftStateMachine::ESubstate::SendOut:
		ProcessSendOut();
		break;
	case ABattleShiftStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleShiftStateMachine::ProcessWait1()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TakeIn;
		Timer = TakeInTime;
	}
}

void ABattleShiftStateMachine::ProcessTakeIn()
{
	Canvas->TakeInPokemonToBall(Timer / TakeInTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::Wait2;
		Timer = WaitTime;
		Canvas->SetBattleMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		Canvas->SetPlayerPokemonBoxActive(false);
		Canvas->SetPlayerPokemonImageActive(false);
	}
}

void ABattleShiftStateMachine::ProcessWait2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::ThrowBall;
		Canvas->PlayThrowedBallAnimation();
	}
}

void ABattleShiftStateMachine::ProcessThrowBall()
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

void ABattleShiftStateMachine::ProcessSendOut()
{
	Canvas->TakeOutPokemonFromBall(Timer / SendOutTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}