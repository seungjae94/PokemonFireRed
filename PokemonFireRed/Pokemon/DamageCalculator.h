#pragma once
#include "Pokemon.h"
#include "PokemonStruct.h"
#include "StatStage.h"
#include "BattleEnums.h"
#include "Battler.h"

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

	static FDamageResult CalcDamage(const UBattler* _Attacker, const UBattler* _Defender);

protected:

private:
	static bool CheckCritical(const UBattler* _Attacker);
	static float GetEffectivePower(const UBattler* _Attacker);
	static float GetEffectiveAtk(const UBattler* _Battler, bool _IsCritical);
	static float GetEffectiveSpAtk(const UBattler* _Battler, bool _IsCritical);
	static float GetEffectiveDef(const UBattler* _Battler, bool _IsCritical);
	static float GetEffectiveSpDef(const UBattler* _Battler, bool _IsCritical);
};

