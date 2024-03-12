#include "BattlePlayerActionSelectStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "EventManager.h"

ABattlePlayerActionSelectStateMachine::ABattlePlayerActionSelectStateMachine()
{
}

ABattlePlayerActionSelectStateMachine::~ABattlePlayerActionSelectStateMachine()
{
}

void ABattlePlayerActionSelectStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::Select;
	Canvas->RefreshMoveSelectBox();
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
		ProcessSelect();
		break;
	case ESubstate::MoveSelect:
		ProcessMoveSelect();
		break;
	case ESubstate::PokemonSelect:
		ProcessPokemonSelect();
		break;
	case ESubstate::ItemSelect:
		ProcessItemSelect();
		break;
	case ESubstate::CantRunMessage1:
		ProcessCantRunMessage1();
		break;
	case ESubstate::CantRunMessage2:
		ProcessCantRunMessage2();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessSelect()
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
			State = ESubstate::ItemSelect;
			Player->SetItemSelectState(EItemSelectState::None);
			UEventManager::FadeChangeLevel(Global::BagUILevel);
			break;
		case Pokemon:
			State = ESubstate::PokemonSelect;
			Player->SetShiftPokemonIndex(-1);
			UEventManager::FadeChangeLevel(Global::PokemonUILevel);
			break;
		case Run:
		{
			if (true == Enemy->IsTrainer())
			{
				State = ESubstate::CantRunMessage1;
				MsgBox->SetMessage(L"No! There's no running\nfrom a TRAINER battle!");
				MsgBox->Write();
				Canvas->SetActionBoxActive(false);
				return;
			}

			State = ESubstate::End;
			Canvas->SetActionBoxActive(false);
			Player->SetAction(EBattleAction::Escape);

			bool RunResult = CalcRunResult();
			Player->SetRunResult(RunResult);
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

void ABattlePlayerActionSelectStateMachine::ProcessMoveSelect()
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();
	int Cursor = Canvas->GetMoveSelectCursor();

	if (true == UEngineInput::IsDown('Z'))
	{
		int SelectMovePP = PlayerPokemon->GetMovePP(Cursor);

		// PP가 0일 경우 기술을 사용할 수 없다.
		if (SelectMovePP == 0)
		{
			return;
		}

		State = ESubstate::End;
		Canvas->SetMoveSelectBoxActive(false);
		Player->SetAction(EBattleAction::Fight);
		Player->SetMoveIndex(Cursor);
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

void ABattlePlayerActionSelectStateMachine::ProcessPokemonSelect()
{
	if (Player->GetShiftPokemonIndex() >= 0)
	{
		if (Player->GetShiftPokemonIndex() == Player->CurPokemonIndex())
		{
			// 포켓몬을 바꾸지 않는 경우 행동을 다시 고른다.
			State = ESubstate::Select;
		}
		else
		{
			// 포켓몬을 바꾸는 경우 PlayerActionSelect 상태를 종료한다.
			Player->SetAction(EBattleAction::Shift);
			State = ESubstate::End;
		}
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessItemSelect()
{
	EItemSelectState ItemSelectState = Player->GetItemSelectState();

	switch (ItemSelectState)
	{
	case EItemSelectState::None:
		break;
	case EItemSelectState::Canceled:
		// 취소한 경우 행동을 다시 고른다.
		State = ESubstate::Select;
		break;
	case EItemSelectState::ItemUsed:
		// 소비 아이템을 사용한 경우 소비 아이템을 사용했다고 마킹하고 PlayerActionSelect 상태를 종료한다.
		Player->SetAction(EBattleAction::UseItem);
		State = ESubstate::End;
		break;
	case EItemSelectState::BallSelected:
		// 몬스터볼을 고른 경우 볼을 골랐다고 마킹하고 PlayerActionSelect 상태를 종료한다.
		Player->SetAction(EBattleAction::Ball);
		State = ESubstate::End;
		break;
	default:
		break;
	}

}

void ABattlePlayerActionSelectStateMachine::ProcessCantRunMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::CantRunMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void ABattlePlayerActionSelectStateMachine::ProcessCantRunMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::Select;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"What will\n" + Player->CurPokemon()->GetNameW() + L" do?");
		MsgBox->Write();
		Canvas->SetActionBoxActive(true);
	}
}

bool ABattlePlayerActionSelectStateMachine::CalcRunResult() const
{
	const UPokemon* PlayerPokemon = Player->CurPokemonReadonly();
	const UPokemon* EnemyPokemon = Enemy->CurPokemonReadonly();

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