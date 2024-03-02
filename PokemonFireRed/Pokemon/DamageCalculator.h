#pragma once
#include "Pokemon.h"
#include "StatStage.h"

enum class ETypeVs
{
	Win,
	Draw,
	Lose,
};

struct FDamageResult
{
public:
	int Damage = 0;
	bool IsCritical = false;
	ETypeVs TypeVs = ETypeVs::Draw;
};

class UDamageCalculator
{
public:
	// constructor destructor
	UDamageCalculator();
	~UDamageCalculator();

	// delete Function
	UDamageCalculator(const UDamageCalculator& _Other) = delete;
	UDamageCalculator(UDamageCalculator&& _Other) noexcept = delete;
	UDamageCalculator& operator=(const UDamageCalculator& _Other) = delete;
	UDamageCalculator& operator=(UDamageCalculator&& _Other) noexcept = delete;

	static FDamageResult CalcDamage(const UPokemon* _Attacker, const UPokemon* _Defender, const UStatStage& _AttackerStatStage, const UStatStage& _DefenderStatStage, EPokemonMove _MoveId);

protected:

private:
	static bool CheckCritical(const UPokemon* _PlayerPokemon, EPokemonMove _MoveId);
	static float GetEffectiveAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical);
};

