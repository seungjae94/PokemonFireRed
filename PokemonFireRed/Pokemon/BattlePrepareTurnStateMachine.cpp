#include "BattlePrepareTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "BattlePlayerShiftStateMachine.h"
#include "BattleEnemyShiftStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ABattlePrepareTurnStateMachine::ABattlePrepareTurnStateMachine()
{
}

ABattlePrepareTurnStateMachine::~ABattlePrepareTurnStateMachine()
{
}

void ABattlePrepareTurnStateMachine::Start()
{
	ABattleStateMachine::Start();

	if (true == Player->CurPokemon()->IsFaint())
	{
		State = ESubstate::SelectPokemonForce;
		Player->SetShiftPokemonIndex(-1);
		UEventManager::FadeChangeLevel(Global::BattleShiftPokemonUILevel);
		return;
	}
	else if (true == Enemy->CurPokemon()->IsFaint())
	{
		State = ESubstate::EnemyShift;
		Enemy->EnemyAutoShift();
		Enemy->ResetTemporalValues();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		BattleEnemyShiftSM->Start();
		return;
	}

	State = ESubstate::End;
}

void ABattlePrepareTurnStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::SelectPokemonForce:
		ProcessSelectPokemonForce();
		break;
	case ESubstate::PlayerShift:
		ProcessPlayerShift();
		break;
	case ESubstate::EnemyShift:
		ProcessEnemyShift();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattlePrepareTurnStateMachine::ProcessSelectPokemonForce()
{
	if (Player->GetShiftPokemonIndex() >= 0)
	{
		State = ESubstate::PlayerShift;
		UPokemon* TakeInPokemon = Player->CurPokemon();

		// 角力 Shift 贸府
		Player->ShiftPokemon();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		// Shift 局聪皋捞记 犁积
		BattlePlayerShiftSM->Start(TakeInPokemon);
	}
}

void ABattlePrepareTurnStateMachine::ProcessPlayerShift()
{
	if (true == BattlePlayerShiftSM->IsEnd())
	{
		if (true == Enemy->CurPokemon()->IsFaint())
		{
			State = ESubstate::EnemyShift;
			return;
		}

		State = ESubstate::End;
	}
}

void ABattlePrepareTurnStateMachine::ProcessEnemyShift()
{
	if (true == BattleEnemyShiftSM->IsEnd())
	{
		State = ESubstate::End;
	}
}

