#include "BattleActionStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleUtil.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "ExpCalculator.h"

ABattleActionStateMachine::ABattleActionStateMachine()
{
}

ABattleActionStateMachine::~ABattleActionStateMachine()
{
}

void ABattleActionStateMachine::Start(UBattler* _Attacker, UBattler* _Defender)
{
	ABattleStateMachine::Start();

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
		BattleMoveSM->Start(Attacker, Defender);
	}
	break;
	case EBattleAction::Escape:
	{
		State = ESubstate::EscapeFail1;
		MsgBox->SetMessage(L"Can't escape!");
		MsgBox->Write();
	}
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::Shift;

		UBattler* Player = _Attacker;
		UBattler* Enemy = _Defender;
		std::wstring TakeInPokemonName = Player->CurPokemon()->GetNameW();

		// 실제 포켓몬 교체
		Player->ShiftPokemon();
		Player->ResetTemporalValues();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		Canvas->SetActionBoxActive(false);
		BattleShiftSM->Start(TakeInPokemonName);
	}
	break;
	case EBattleAction::UseItem:
	{
		State = ESubstate::End;
		Canvas->SetActionBoxActive(false);
	}
	break;
	case EBattleAction::Ball:
	{
		State = ESubstate::UseItem;
		MsgBox->SetMessage(L"Not Implemented Yet!");
		MsgBox->Write();
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
	case ABattleActionStateMachine::ESubstate::EscapeFail1:
		ProcessEscapeFail1();
		break;
	case ABattleActionStateMachine::ESubstate::EscapeFail2:
		ProcessEscapeFail2();
		break;
	case ABattleActionStateMachine::ESubstate::Shift:
		ProcessShift();
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

void ABattleActionStateMachine::ProcessEscapeFail1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::EscapeFail2;
		MsgBox->ShowSkipArrow();
	}
}

void ABattleActionStateMachine::ProcessEscapeFail2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = ESubstate::End;
	}

}

void ABattleActionStateMachine::ProcessShift()
{
	if (true == BattleShiftSM->IsEnd())
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