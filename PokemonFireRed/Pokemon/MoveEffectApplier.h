#pragma once
#include "Battler.h"

class UMoveEffectApplier
{
public:
	// constructor destructor
	UMoveEffectApplier();
	~UMoveEffectApplier();

	// delete Function
	UMoveEffectApplier(const UMoveEffectApplier& _Other) = delete;
	UMoveEffectApplier(UMoveEffectApplier&& _Other) noexcept = delete;
	UMoveEffectApplier& operator=(const UMoveEffectApplier& _Other) = delete;
	UMoveEffectApplier& operator=(UMoveEffectApplier&& _Other) noexcept = delete;

	static void ApplyBE(UBattler* _Attacker, UBattler* _Defender);
	static void ApplySE(UBattler* _Attacker, UBattler* _Defender);

protected:

private:
	static void Apply(
		UBattler* _Target,
		EStatStageChangeType _StatStageId,
		int _StatStageValue,
		EPokemonStatus _StatusId
	);
};

