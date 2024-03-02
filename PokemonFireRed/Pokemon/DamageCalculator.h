#pragma once
#include "Pokemon.h"
#include "PokemonStruct.h"
#include "StatStage.h"
#include "BattleEnums.h"

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
	static float GetEffectivePower(const UPokemon* _Attacker, EPokemonMove _MoveId);
	static float GetEffectiveAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical);
	static float GetEffectiveSpAtk(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical);
	static float GetEffectiveDef(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical);
	static float GetEffectiveSpDef(const UPokemon* _Pokemon, const UStatStage& _StatStage, bool _IsCritical);
};

