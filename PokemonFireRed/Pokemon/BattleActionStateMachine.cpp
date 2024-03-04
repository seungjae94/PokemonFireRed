#include "BattleActionStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleUtil.h"

ABattleActionStateMachine::ABattleActionStateMachine()
{
}

ABattleActionStateMachine::~ABattleActionStateMachine()
{
}

void ABattleActionStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Attacker, UBattler* _Defender)
{
	Canvas = _Canvas;
	Attacker = _Attacker;
	Defender = _Defender;

	const FPokemonMove* Move = Attacker->CurMove();
	const UPokemon* AttackerPokemon = Attacker->CurPokemonReadonly();
	EBattleAction AttackerAction = Attacker->CurAction();

	switch (AttackerAction)
	{
	case EBattleAction::Fight:
	{
		State = ESubstate::Move;
		BattleMoveSM->Start(Canvas, Attacker, Defender);
	}
		break;
	case EBattleAction::Escape:
	{
		State = ESubstate::EscapeFail;
		Canvas->SetBattleMessage(L"Can't escape!");
		Timer = BattleMsgShowTime;
	}
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::Switch;
		Canvas->SetBattleMessage(L"Not Implemented Yet!");
	}
	break;
	case EBattleAction::Item:
	{
		State = ESubstate::UseItem;
		Canvas->SetBattleMessage(L"Not Implemented Yet!");
	}
	break;
	default:
		break;
	}
}

void ABattleActionStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleActionStateMachine::ESubstate::None:
		break;
	case ABattleActionStateMachine::ESubstate::EscapeFail:
		ProcessEscapeFail();
		break;
	case ABattleActionStateMachine::ESubstate::Switch:
		ProcessSwitch();
		break;
	case ABattleActionStateMachine::ESubstate::UseItem:
		ProcessItem();
		break;
	case ABattleActionStateMachine::ESubstate::Move:
		ProcessMove();
		break;
	case ABattleActionStateMachine::ESubstate::Faint:
		ProcessFaint();
		break;
	case ABattleActionStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleActionStateMachine::ProcessEscapeFail()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattleActionStateMachine::ProcessSwitch()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattleActionStateMachine::ProcessItem()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattleActionStateMachine::ProcessMove()
{
	if (true == BattleMoveSM->IsEnd())
	{
		// 기절한 포켓몬이 있을 경우 기절 애니메이션을 재생해준다.
		const UPokemon* AttackerPokemon = Attacker->CurPokemon();
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		Fainters.clear();
		bool AttackerFaint = (AttackerPokemon->GetStatusId() == EPokemonStatus::Faint);
		bool DefenderFaint = (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint);

		if (true == AttackerFaint)
		{
			Fainters.push_back(Attacker);
		}
		
		if (true == DefenderFaint)
		{
			Fainters.push_back(Defender);
		}

		if (Fainters.size() > 0)
		{
			StateChangeToFaint();
			return;
		}

		State = ESubstate::End;
	}
}

void ABattleActionStateMachine::ProcessFaint()
{
	if (FaintState == EFaintState::HidePokemon && true == Fainter->IsPlayer())
	{
		Canvas->LerpFaintPlayerPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			FaintState = EFaintState::ShowFaintMessage;
			Canvas->SetPlayerPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
		}
	}
	else if (FaintState == EFaintState::HidePokemon)
	{
		Canvas->LerpFaintEnemyPokemon(Timer / FaintTime);

		if (Timer <= 0.0f)
		{
			FaintState = EFaintState::ShowFaintMessage;
			Canvas->SetEnemyPokemonBoxActive(false);
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Fainter) + L"\nfainted!");
		}
	}
	else if (true == UEngineInput::IsDown('Z'))
	{
		if (true == Fainters.empty())
		{
			State = ESubstate::End;
		}
		else
		{
			StateChangeToFaint();
		}
	}
}

void ABattleActionStateMachine::StateChangeToFaint()
{
	State = ESubstate::Faint;
	FaintState = EFaintState::HidePokemon;
	Timer = FaintTime;
	Fainter = Fainters.front();
	Fainters.pop_front();
}
