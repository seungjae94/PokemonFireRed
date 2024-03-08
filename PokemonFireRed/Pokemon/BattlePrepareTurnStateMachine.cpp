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

void ABattlePrepareTurnStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Player = _Player;
	Enemy = _Enemy;

	if (true == Player->CurPokemon()->IsFaint())
	{
		State = ESubstate::SelectPokemonForce;
		Player->SetShiftPokemonIndex(-1);
		UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		return;
	}
	else if (true == Enemy->CurPokemon()->IsFaint())
	{
		State = ESubstate::EnemyShift;
		Enemy->EnemyAutoShift();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		BattleEnemyShiftSM->Start(Canvas, MsgBox, Enemy);
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
		std::wstring TakeInPokemonName = Player->CurPokemon()->GetNameW();

		// 角力 Shift 贸府
		Player->ShiftPokemon();
		Player->GetParticipants().push_back(Enemy->CurPokemon());
		Enemy->GetParticipants().push_back(Player->CurPokemon());

		// Shift 局聪皋捞记 犁积
		BattlePlayerShiftSM->Start(TakeInPokemonName, Canvas, MsgBox, Player);
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

