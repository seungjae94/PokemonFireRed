#include "BattlePlayerShiftStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "SoundManager.h"

ABattlePlayerShiftStateMachine::ABattlePlayerShiftStateMachine() 
{
}

ABattlePlayerShiftStateMachine::~ABattlePlayerShiftStateMachine() 
{
}

void ABattlePlayerShiftStateMachine::Start(UPokemon* _TakeInPokemon)
{
	ABattleStateMachine::Start();

	TakeInPokemonName = _TakeInPokemon->GetNameW();

	if (false == _TakeInPokemon->IsFaint())
	{
		State = ESubstate::TakeInMessage1;
		Canvas->PlayerUIReadyForShift();
		MsgBox->SetMessage(TakeInPokemonName + L", that's enough!\nCome back!");
		MsgBox->Write();
	}
	else
	{
		State = ESubstate::SendOutMessage1;
		Canvas->SetPlayerPokemonBoxActive(false);
		Canvas->SetPlayerPokemonImageActive(false);
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
	}
}

void ABattlePlayerShiftStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::TakeInMessage1:
		ProcessTakeInMessage1();
		break;
	case ESubstate::TakeInMessage2:
		ProcessTakeInMessage2();
		break;
	case ESubstate::TakeIn:
		ProcessTakeIn();
		break;
	case ESubstate::SendOutMessage1:
		ProcessSendOutMessage1();
		break;
	case ESubstate::SendOutMessage2:
		ProcessSendOutMessage2();
		break;
	case ESubstate::ThrowBall:
		ProcessThrowBall();
		break;
	case ESubstate::SendOut:
		ProcessSendOut();
		break;
	case ESubstate::ShowPlayerBox:
		ProcessShowPlayerBox();
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

void ABattlePlayerShiftStateMachine::ProcessTakeInMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::TakeInMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessTakeInMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TakeIn;
		Timer = TakeInTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessTakeIn()
{
	Canvas->TakeInPlayerPokemonToBall(Timer / TakeInTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::SendOutMessage1;
		Canvas->SetPlayerPokemonBoxActive(false);
		Canvas->SetPlayerPokemonImageActive(false);
		MsgBox->SetMessage(L"Go! " + Player->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();
	}
}

void ABattlePlayerShiftStateMachine::ProcessSendOutMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::SendOutMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessSendOutMessage2()
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
		USoundManager::PlaySE(Player->CurPokemon()->GetCrySoundName());
		Canvas->SetThrowedBallActive(false);
		Canvas->SetPlayerPokemonBoxActive(true);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshPlayerPokemonImage();
		Canvas->InitMoveSelectBox();
		Canvas->SetPlayerPokemonImageActive(true);
	}
}

void ABattlePlayerShiftStateMachine::ProcessSendOut()
{
	Canvas->TakeOutPlayerPokemonFromBall(Timer / SendOutTime);

	if (Timer <= 0.0f && MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::ShowPlayerBox;
		Timer = ShowPlayerBoxTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessShowPlayerBox()
{
	Canvas->LerpShowPlayerPokemonBox(Timer / ShowPlayerBoxTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::EndWait;
		Timer = EndWaitTime;
	}
}

void ABattlePlayerShiftStateMachine::ProcessEndWait()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}
