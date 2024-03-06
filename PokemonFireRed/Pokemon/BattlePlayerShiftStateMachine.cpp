#include "BattlePlayerShiftStateMachine.h"

ABattlePlayerShiftStateMachine::ABattlePlayerShiftStateMachine() 
{
}

ABattlePlayerShiftStateMachine::~ABattlePlayerShiftStateMachine() 
{
}

void ABattlePlayerShiftStateMachine::Start(std::wstring_view _TakeInPokemonName, ABattleCanvas* _Canvas, const UBattler* _Player)
{
	TakeInPokemonName = _TakeInPokemonName;
	Canvas = _Canvas;
	Player = _Player;

	State = ESubstate::TakeIn;
	Timer = WaitTime;
	Canvas->SetBattleMessage(TakeInPokemonName + L", that's enough!\nCome back!");
}

void ABattlePlayerShiftStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattlePlayerShiftStateMachine::ESubstate::None:
		break;
	case ESubstate::Wait1:
		ProcessWait1();
		break;
	case ABattlePlayerShiftStateMachine::ESubstate::TakeIn:
		ProcessTakeIn();
		break;
	case ESubstate::Wait2:
		ProcessWait2();
		break;
	case ABattlePlayerShiftStateMachine::ESubstate::ThrowBall:
		ProcessThrowBall();
		break;
	case ABattlePlayerShiftStateMachine::ESubstate::SendOut:
		ProcessSendOut();
		break;
	case ABattlePlayerShiftStateMachine::ESubstate::End:
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

	if (Timer <= 0.0f)
	{
		State = ESubstate::Wait2;
		Timer = WaitTime;
		Canvas->SetBattleMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
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

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}