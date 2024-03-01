#pragma once
#include "Pokemon.h"

enum class EEnemyType;

enum class EBattleEnemyAction
{
	None,
	Move,
	Shift,
	Item,
};

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

	static EBattleEnemyAction Generate(EEnemyType _EnemyType, const UPokemon* _EnemyPokemon);
	static int GetGeneratedMoveIndex() 
	{
		return MoveIndex;
	}

protected:

private:
	static int MoveIndex;
};