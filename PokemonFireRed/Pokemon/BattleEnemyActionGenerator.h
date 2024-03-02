#pragma once
#include "Pokemon.h"
#include "BattleEnums.h"

class UBattleEnemyActionGenerator
{
public:
	// constructor destructor
	UBattleEnemyActionGenerator();
	~UBattleEnemyActionGenerator();

	// delete Function
	UBattleEnemyActionGenerator(const UBattleEnemyActionGenerator& _Other) = delete;
	UBattleEnemyActionGenerator(UBattleEnemyActionGenerator&& _Other) noexcept = delete;
	UBattleEnemyActionGenerator& operator=(const UBattleEnemyActionGenerator& _Other) = delete;
	UBattleEnemyActionGenerator& operator=(UBattleEnemyActionGenerator&& _Other) noexcept = delete;

	static EBattleAction Generate(EEnemyType _EnemyType, const UPokemon* _EnemyPokemon);
	static int GetGeneratedMoveIndex() 
	{
		return MoveIndex;
	}

protected:

private:
	static int MoveIndex;
};