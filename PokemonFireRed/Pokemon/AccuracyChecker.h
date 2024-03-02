#pragma once
#include "Pokemon.h"
#include "StatStage.h"

class UAccuracyChecker
{
public:
	// constructor destructor
	UAccuracyChecker();
	~UAccuracyChecker();

	// delete Function
	UAccuracyChecker(const UAccuracyChecker& _Other) = delete;
	UAccuracyChecker(UAccuracyChecker&& _Other) noexcept = delete;
	UAccuracyChecker& operator=(const UAccuracyChecker& _Other) = delete;
	UAccuracyChecker& operator=(UAccuracyChecker&& _Other) noexcept = delete;

	static bool Check(const UPokemon* _Attacker, const UPokemon* _Defender, const UStatStage& _AttackerStatStage, const UStatStage& _DefenderStatStage, int _AttackMoveIndex);

protected:

private:

};

