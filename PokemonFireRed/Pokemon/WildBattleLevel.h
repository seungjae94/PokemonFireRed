#pragma once
#include "PokemonLevel.h"
#include "WildBattleCanvas.h"

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
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;
private:
	UPokemon EnemyPokemon;
	AWildBattleCanvas* Canvas = nullptr;
};

