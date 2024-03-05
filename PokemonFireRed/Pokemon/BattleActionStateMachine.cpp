#include "BattleActionStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleUtil.h"
#include "ExpCalculator.h"

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
	AActor::Tick(_DeltaTime);

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
		State = ESubstate::End;
	}
}