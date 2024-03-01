#include "BattlePlayerActionStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"

ABattlePlayerActionStateMachine::ABattlePlayerActionStateMachine()
{
}

ABattlePlayerActionStateMachine::~ABattlePlayerActionStateMachine()
{
}

void ABattlePlayerActionStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattlePlayerActionStateMachine::ESubstate::None:
		break;
	case ABattlePlayerActionStateMachine::ESubstate::Select:
		ProcessSelect(_DeltaTime);
		break;
	case ABattlePlayerActionStateMachine::ESubstate::ShowEscapeSuccessMsg:
		break;
	case ABattlePlayerActionStateMachine::ESubstate::ShowEscapeFailMsg:
		break;
	case ABattlePlayerActionStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePlayerActionStateMachine::ProcessSelect(float _DeltaTime)
{
	int Cursor = Canvas->GetActionCursor();

	if (true == UEngineInput::IsDown('Z'))
	{
		switch (Cursor)
		{
		case Fight:
			break;
		case Bag:
			break;
		case Pokemon:
			break;
		case Run:
			break;
		default:
			break;
		}
	}


	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		if (Cursor % 2 == 1)
		{
			Canvas->SetActionCursor(Cursor - 1);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		if (Cursor % 2 == 0)
		{
			Canvas->SetActionCursor(Cursor + 1);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		if (Cursor >= 2)
		{
			Canvas->SetActionCursor(Cursor - 2);
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (Cursor < 2)
		{
			Canvas->SetActionCursor(Cursor + 2);
			return;
		}
	}
}
