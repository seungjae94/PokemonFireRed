#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(
	ABattleCanvas* _Canvas,
	UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon,
	UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage,
	EBattleAction _PlayerAction, int _PlayerMoveIndex
)
{
	// 입력 데이터 저장
	Canvas = _Canvas;
	PlayerPokemon = _PlayerPokemon;
	EnemyPokemon = _EnemyPokemon;
	PlayerStatStage = &_PlayerStatStage;
	EnemyStatStage = &_EnemyStatStage;
	PlayerAction = _PlayerAction;
	PlayerMoveId = PlayerPokemon->GetMoveId(_PlayerMoveIndex);

	// 적 액션 생성
	GenerateEnemyAction();

	// 턴 순서 결정
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(PlayerAction, EnemyAction, PlayerPokemon, EnemyPokemon, PlayerMoveId, EnemyMoveId, *PlayerStatStage, *EnemyStatStage);
	IsFirstTurnEnd = false;

	// 상태 변경
	if (true == IsPlayerFirst)
	{
		DispatchPlayerTurn();
	}
	else
	{
		DispatchEnemyTurn();
	}
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerMoveFail:
		break;
	case ABattleTurnStateMachine::ESubstate::EnemyMoveFail:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerMove:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerMoveSecondaryEffect:
		break;
	case ABattleTurnStateMachine::ESubstate::EnemyMove:
		break;
	case ABattleTurnStateMachine::ESubstate::EnemyMoveSecondaryEffect:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerEndOfTurn:
		break;
	case ABattleTurnStateMachine::ESubstate::EnemyEndOfTurn:
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::GenerateEnemyAction()
{
	// 상대 행동 생성
	EnemyAction = UBattleEnemyActionGenerator::Generate(EEnemyType::Wild, EnemyPokemon);
	int EnemyMoveIndex = UBattleEnemyActionGenerator::GetGeneratedMoveIndex();
	EnemyMoveId = EnemyPokemon->GetMoveId(EnemyMoveIndex);
}

void ABattleTurnStateMachine::DispatchPlayerTurn()
{
	switch (PlayerAction)
	{
	case EBattleAction::Fight:
		DispatchPlayerFight();
		break;
	case EBattleAction::Escape:
	{
		State = ESubstate::PlayerEscapeFail;
		Canvas->SetBattleMessage(L"Can't Escape!");
	}
		break;
	case EBattleAction::Shift:
	{
		State = ESubstate::PlayerSwitch;
		Canvas->SetBattleMessage(L"Debug - You Changed Pokemon!");
	}
		break;
	case EBattleAction::Item:
	{
		State = ESubstate::PlayerUseItem;
		Canvas->SetBattleMessage(L"Debug - You used Item!");
	}
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchPlayerFight()
{
	bool AccuracyCheckResult = UAccuracyChecker::Check(PlayerPokemon, EnemyPokemon, *PlayerStatStage, *EnemyStatStage, PlayerMoveId);

	if (false == AccuracyCheckResult)
	{
		Canvas->SetBattleMessage(PlayerPokemon->GetNameW() + L"'s\nattack failed!");
		State = ESubstate::PlayerMoveFail;
		Timer = MoveFailMsgShowTime;
		return;
	}

	State = ESubstate::PlayerMove;
	const FPokemonMove* Move = UPokemonDB::FindMove(PlayerMoveId);
	Canvas->SetBattleMessage(PlayerPokemon->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	
	// Result = UDamageCalculator::CalcDamage(PlayerPokemon, EnemyPokemon, ...);
}

void ABattleTurnStateMachine::DispatchEnemyTurn()
{
	switch (EnemyAction)
	{
	case EBattleAction::Fight:
		DispatchEnemyFight();
		break;
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::EnemySwitch;
		Canvas->SetBattleMessage(L"Debug - Enemy Changed Pokemon!");
	}
	break;
	case EBattleAction::Item:
	{
		State = ESubstate::EnemyUseItem;
		Canvas->SetBattleMessage(L"Debug - Enemy used Item!");
	}
	break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchEnemyFight()
{
	bool AccuracyCheckResult = UAccuracyChecker::Check(EnemyPokemon, PlayerPokemon, *EnemyStatStage, *PlayerStatStage, EnemyMoveId);

	if (false == AccuracyCheckResult)
	{
		Canvas->SetBattleMessage(EnemyPokemon->GetNameW() + L"'s\nattack failed!");
		State = ESubstate::EnemyMoveFail;
		Timer = MoveFailMsgShowTime;
		return;
	}

	State = ESubstate::EnemyMove;
	const FPokemonMove* Move = UPokemonDB::FindMove(EnemyMoveId);
	Canvas->SetBattleMessage(EnemyPokemon->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	
	// Result = UDamageCalculator::CalcDamage(EnemyPokemon, PlayerPokemon, ...);
}

