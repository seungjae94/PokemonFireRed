#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"

class UBattleLevel;
class ABattleCanvas;
 
class ABattleTurnStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		PlayerEscapeFail,
		PlayerSwitch,
		PlayerUseItem,
		PlayerMoveFail,
		PlayerMove,
		PlayerMoveSecondaryEffect,
		EnemySwitch,
		EnemyUseItem,
		EnemyMoveFail,
		EnemyMove,
		EnemyMoveSecondaryEffect,
		PlayerEndOfTurn,
		EnemyEndOfTurn,
		End,
	};
public:
	// constructor destructor
	ABattleTurnStateMachine();
	~ABattleTurnStateMachine();

	// delete Function
	ABattleTurnStateMachine(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine(ABattleTurnStateMachine&& _Other) noexcept = delete;
	ABattleTurnStateMachine& operator=(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine& operator=(ABattleTurnStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(
		ABattleCanvas* _Canvas, 
		UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon, 
		UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage, 
		EBattleAction _PlayerAction, int _PlayerMoveIndex
	);

	void Reset() {};

protected:

private:
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	UPokemon* PlayerPokemon = nullptr; 
	UPokemon* EnemyPokemon = nullptr;
	UStatStage* PlayerStatStage = nullptr; 
	UStatStage* EnemyStatStage = nullptr;
	EBattleAction PlayerAction = EBattleAction::None;
	EBattleAction EnemyAction = EBattleAction::None;
	EPokemonMove PlayerMoveId = EPokemonMove::None;
	EPokemonMove EnemyMoveId = EPokemonMove::None;

	// 고유 데이터
	ESubstate State = ESubstate::None;
	const float MoveFailMsgShowTime = 1.5f;
	float Timer = 0.0f;
	bool IsPlayerFirst = true;
	bool IsFirstTurnEnd = false;
	
	FDamageResult Result;

	// 로직
	void Tick(float _DeltaTime) override;

	void GenerateEnemyAction();
	void DispatchPlayerTurn();
	void DispatchPlayerFight();
	void DispatchEnemyTurn();
	void DispatchEnemyFight();
};

