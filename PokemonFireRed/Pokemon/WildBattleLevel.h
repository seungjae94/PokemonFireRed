#pragma once
#include "PokemonLevel.h"
#include "WildBattleCanvas.h"

enum class EBattleState
{
	BattleStart,
	PlayerAction,
	PlayerMove,
	EnemyMove,
	PlayerSecondaryEffect,
	EnemySecondaryEffect,
	BattleEnd
};

enum class EBattleStartSubstate
{
	FadeWait,
	GroundMove,
	EnemyPokemonBoxMove,
	ZClickWait,
	PlayerBattlerThrow,
	PlayerPokemonTakeout,
	PlayerPokemonBoxMove,
};

class UWildBattleLevel : public UPokemonLevel
{
public:
	// constructor destructor
	UWildBattleLevel();
	~UWildBattleLevel();

	// delete Function
	UWildBattleLevel(const UWildBattleLevel& _Other) = delete;
	UWildBattleLevel(UWildBattleLevel&& _Other) noexcept = delete;
	UWildBattleLevel& operator=(const UWildBattleLevel& _Other) = delete;
	UWildBattleLevel& operator=(UWildBattleLevel&& _Other) noexcept = delete;

protected:
private:
	UPokemon EnemyPokemon;
	AWildBattleCanvas* Canvas = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart(float _DeltaTime);
	void ProcessBattleStartFadeWait(float _DeltaTime);
	void ProcessBattleStartGroundMove(float _DeltaTime);
	void ProcessBattleStartEnemyPokemonBoxMove(float _DeltaTime);
	void ProcessBattleStartZClickWait(float _DeltaTime);
	void ProcessBattleStartPlayerBattlerThrow(float _DeltaTime);
	void ProcessBattleStartPlayerPokemonTakeout(float _DeltaTime);
	void ProcessBattleStartPlayerPokemonBoxMove(float _DeltaTime);

	// FSM
	EBattleState State = EBattleState::BattleStart;
	float Timer = 0.0f;

	// FSM - BattleStart
	EBattleStartSubstate BattleStartSubstate = EBattleStartSubstate::FadeWait;
	float FadeWaitTime = 0.5f;
	float GroundMoveTime = 1.5f;
	float EnemyPokemonBoxMoveTime = 0.5f;
	float PlayerBattleThrowTime = 1.3f;
	float BallThrowMotionWaitTime = 0.5f;
	bool BallThrowAnimationPlayed = false;
	float PlayerPokemonTakeoutTime = 0.3f;
	float PlayerPokemonBoxMoveTime = 0.5f;
};

