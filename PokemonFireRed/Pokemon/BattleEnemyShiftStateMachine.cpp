#include "BattleEnemyShiftStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "Battler.h"

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

void ABattleEnemyShiftStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Enemy)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Enemy = _Enemy;

	State = ESubstate::EntryArrowMove;
	Timer = EnemyArrowMoveTime;
	Canvas->RefreshEnemyEntry();
	Canvas->SetEnemyEntryArrowAlpha(1.0f);
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
		Canvas->SetEnemyPokemonBoxActive(true);
		Canvas->RefreshEnemyPokemonBox();
	}
}

void ABattleEnemyShiftStateMachine::ProcessPokemonBoxMove()
{
	Canvas->LerpShowEnemyPokemonBox(Timer / EnemyPokemonBoxMoveTime);

	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}
