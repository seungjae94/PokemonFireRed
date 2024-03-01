#include "BattlePlayerActionSelectStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"

ABattlePlayerActionSelectStateMachine::ABattlePlayerActionSelectStateMachine()
{
}

ABattlePlayerActionSelectStateMachine::~ABattlePlayerActionSelectStateMachine()
{
}

void ABattlePlayerActionSelectStateMachine::Start(ABattleCanvas* _Canvas, const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon)
{
	Canvas = _Canvas;
	PlayerPokemon = _PlayerPokemon;
	EnemyPokemon = _EnemyPokemon;
	State = ESubstate::Select;
}

void ABattlePlayerActionSelectStateMachine::Reset()
{
	RunAttemptCount = 0;
}

void ABattlePlayerActionSelectStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::Select:
		ProcessSelect(_DeltaTime);
		break;
	case ESubstate::MoveSelect:
		ProcessMoveSelect(_DeltaTime);
		break;
	case ESubstate::ShowEscapeSuccessMsg:
		ProcessShowEscapeSuccessMsg(_DeltaTime);
		break;
	case ESubstate::ShowEscapeFailMsg:
		ProcessShowEscapeFailMsg(_DeltaTime);
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessSelect(float _DeltaTime)
{
	int Cursor = Canvas->GetActionCursor();

	if (true == UEngineInput::IsDown('Z'))
	{
		switch (Cursor)
		{
		case Fight:
			State = ESubstate::MoveSelect;
			Canvas->SetActionBoxActive(false);
			Canvas->SetMoveSelectBoxActive(true);
			break;
		case Bag:
			break;
		case Pokemon:
			break;
		case Run:
		{
			bool RunResult = CalcRunResult();

			if (true == RunResult)
			{
				State = ESubstate::ShowEscapeSuccessMsg;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Got away safely!");
			}
			else
			{
				State = ESubstate::ShowEscapeFailMsg;
				Canvas->SetActionBoxActive(false);
				Canvas->SetBattleMessage(L"Can't escape!");
			}
		}
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

bool ABattlePlayerActionSelectStateMachine::CalcRunResult()
{
	int PSpeed = PlayerPokemon->GetSpeed();
	int ESpeed = EnemyPokemon->GetSpeed();

	if (PSpeed >= ESpeed)
	{
		return true;
	}

	int RandomNumber = UPokemonMath::RandomInt(0, 255);

	int RHS = UPokemonMath::Floor(PSpeed * 128.0f / ESpeed);
	RHS += 30 * RunAttemptCount;
	RHS = UPokemonMath::Mod(RHS, 256);
	return RandomNumber < RHS;
}

void ABattlePlayerActionSelectStateMachine::ProcessMoveSelect(float _DeltaTime)
{
	int Cursor = Canvas->GetMoveSelectCursor();

	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::End;
		ActionResult = EBattlePlayerAction::Fight;
		SelectedMove = Cursor;
		Canvas->SetMoveSelectBoxActive(false);
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		State = ESubstate::Select;
		Canvas->SetActionBoxActive(true);
		Canvas->SetMoveSelectBoxActive(false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		if (Cursor % 2 == 1)
		{
			Canvas->SetMoveSelectCursor(Cursor - 1);
		}
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		if (Cursor % 2 == 0 && (Cursor + 1) < PlayerPokemon->GetMoveCount())
		{
			Canvas->SetMoveSelectCursor(Cursor + 1);
		}
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		if (Cursor / 2 == 1)
		{
			Canvas->SetMoveSelectCursor(Cursor - 2);
		}
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (Cursor / 2 == 0 && (Cursor + 2) < PlayerPokemon->GetMoveCount())
		{
			Canvas->SetMoveSelectCursor(Cursor + 2);
		}
		return;
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessShowEscapeSuccessMsg(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ActionResult = EBattlePlayerAction::EscapeSuccess;
		State = ESubstate::End;
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessShowEscapeFailMsg(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		ActionResult = EBattlePlayerAction::EscapeFail;
		State = ESubstate::End;
	}
}
