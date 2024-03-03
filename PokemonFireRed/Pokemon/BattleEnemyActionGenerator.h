#pragma once
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

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

	static void Generate(UBattler* _Enemy);

protected:

private:

};