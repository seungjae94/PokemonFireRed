#pragma once
#include <EngineCore/Actor.h>
#include "Animator.h"

class UBattler;
enum class EPokemonMove;

class AAnimatorGenerator : public AActor
{
public:
	// constructor destructor
	AAnimatorGenerator();
	~AAnimatorGenerator();

	// delete Function
	AAnimatorGenerator(const AAnimatorGenerator& _Other) = delete;
	AAnimatorGenerator(AAnimatorGenerator&& _Other) noexcept = delete;
	AAnimatorGenerator& operator=(const AAnimatorGenerator& _Other) = delete;
	AAnimatorGenerator& operator=(AAnimatorGenerator&& _Other) noexcept = delete;

	AAnimator* GenerateMoveAnimator(UBattler* _Attacker, EPokemonMove _MoveId);
	AAnimator* GenerateStatStageEffectAnimator(UBattler* _Target, bool _IsStatDown);

protected:

private:
};

