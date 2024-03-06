#include "BattlePrepareTurnStateMachine.h"
#include "EventManager.h"
#include "BattlePlayerShiftStateMachine.h";

ABattlePrepareTurnStateMachine::ABattlePrepareTurnStateMachine()
{
}

ABattlePrepareTurnStateMachine::~ABattlePrepareTurnStateMachine()
{
}

void ABattlePrepareTurnStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy)
{
	Canvas = _Canvas;
	Player = _Player;
	Enemy = _Enemy;

	if (Player->CurPokemon()->IsFaint())
	{
		State = ESubstate::SelectPokemonForce;
		Player->SetShiftPokemonIndex(-1);
		UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		return;
	}
	else if (Enemy->CurPokemon()->IsFaint())
	{
		State = ESubstate::EnemyAboutToUseMessage1;
		return;
	}
	
	State = ESubstate::End;
}

void ABattlePrepareTurnStateMachine::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	switch (State)
	{
	case ABattlePrepareTurnStateMachine::ESubstate::None:
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::SelectPokemonForce:
		ProcessSelectPokemonForce();
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::EnemyAboutToUseMessage1:
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::EnemyAboutToUseMessage2:
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::SelectPokemonOptional:
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::PlayerShift:
		ProcessPlayerShift();
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::EnemyShift:
		break;
	case ABattlePrepareTurnStateMachine::ESubstate::End:
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
		std::wstring TakeInPokemonName = Player->CurPokemon()->GetNameW();

		// 角力 Shift 贸府
		Player->ShiftPokemon();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		// Shift 局聪皋捞记 犁积
		BattlePlayerShiftSM->Start(TakeInPokemonName, Canvas, Player);
	}
}

void ABattlePrepareTurnStateMachine::ProcessPlayerShift()
{
	if (true == BattlePlayerShiftSM->IsEnd())
	{
		if (true == Enemy->CurPokemon()->IsFaint())
		{
			State = ESubstate::EnemyAboutToUseMessage1;
			return;
		}

		State = ESubstate::End;
	}
}

