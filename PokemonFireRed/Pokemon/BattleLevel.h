#pragma once
#include "PokemonLevel.h"
#include "BattleCanvas.h"
#include "BattleStartStateManchine.h"

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

class UBattleLevel : public UPokemonLevel
{
public:
	// constructor destructor
	UBattleLevel();
	~UBattleLevel();

	// delete Function
	UBattleLevel(const UBattleLevel& _Other) = delete;
	UBattleLevel(UBattleLevel&& _Other) noexcept = delete;
	UBattleLevel& operator=(const UBattleLevel& _Other) = delete;
	UBattleLevel& operator=(UBattleLevel&& _Other) noexcept = delete;

protected:
private:
	UPokemon EnemyPokemon;
	ABattleCanvas* Canvas = nullptr;
	ABattleStartStateManchine* BSSM = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;

	void ProcessBattleStart(float _DeltaTime);

	// FSM
	EBattleState State = EBattleState::BattleStart;
	float Timer = 0.0f;
};

