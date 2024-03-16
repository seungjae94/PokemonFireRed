#include "BattleEnemyShiftStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "Battler.h"
#include "SoundManager.h"

ABattleEnemyShiftStateMachine::ABattleEnemyShiftStateMachine()
{
}

ABattleEnemyShiftStateMachine::~ABattleEnemyShiftStateMachine()
{
}

void ABattleEnemyShiftStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::EntryArrowMove:
		ProcessEntryArrowMove();
		break;
	case ESubstate::PokemonAppearWait:
		ProcessPokemonAppearWait();
		break;
	case ESubstate::PokemonAppear:
		ProcessPokemonAppear(_DeltaTime);
		break;
	case ESubstate::PokemonBoxMove:
		ProcessPokemonBoxMove();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleEnemyShiftStateMachine::Start()
{
	ABattleStateMachine::Start();

	State = ESubstate::EntryArrowMove;
	Timer = EnemyArrowMoveTime;
	Canvas->RefreshEnemyEntry();
	Canvas->EnemyUIReadyForShift();
}

void ABattleEnemyShiftStateMachine::ProcessEntryArrowMove()
{
	Canvas->LerpShowEnemyEntryArrow(Timer / EnemyArrowMoveTime);

	if (Timer <= 0)
	{
		State = ESubstate::PokemonAppearWait;
		MsgBox->SetMessage(Enemy->GetTrainerNameW() + L" sent\nout " + Enemy->CurPokemonReadonly()->GetNameW() + L"!");
		MsgBox->Write();

		Timer = EnemyPokemonAppearWaitTime;
	}
}

void ABattleEnemyShiftStateMachine::ProcessPokemonAppearWait()
{
	if (Timer <= 0)
	{
		State = ESubstate::PokemonAppear;
		USoundManager::PlaySE(Enemy->CurPokemon()->GetCrySoundName());
		Canvas->PlayEnemyGroundBallAnimation();
		Canvas->RefreshEnemyPokemonImage();
		EntryFadeTimer = EntryFadeTime;
		EnemyPokemonAppearTimer = EnemyPokemonAppearTime;
	}
}

void ABattleEnemyShiftStateMachine::ProcessPokemonAppear(float _DeltaTime)
{
	EntryFadeTimer -= _DeltaTime;
	EnemyPokemonAppearTimer -= _DeltaTime;

	if (true == Canvas->IsEnemyGroundBallAnimationEnd())
	{
		Canvas->SetEnemyGroundBallActive(false);
	}

	Canvas->LerpHideEnemyEntry(EntryFadeTimer / EntryFadeTime);
	Canvas->TakeOutEnemyPokemonFromBall(EnemyPokemonAppearTimer / EnemyPokemonAppearTime);

	if (EntryFadeTimer <= 0.0f
		&& EnemyPokemonAppearTimer <= 0.0f
		&& MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::PokemonBoxMove;
		Timer = EnemyPokemonBoxMoveTime;
		Canvas->RefreshEnemyPokemonBox();
	}
}

void ABattleEnemyShiftStateMachine::ProcessPokemonBoxMove()
{
	if (false == Canvas->IsEnemyPokemonBoxActive())
	{
		Canvas->SetEnemyPokemonBoxActive(true);
	}
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}
