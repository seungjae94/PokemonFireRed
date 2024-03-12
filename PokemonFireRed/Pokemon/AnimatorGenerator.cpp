#include "AnimatorGenerator.h"
#include "Battler.h"
#include "Pokemon.h"
#include "PlayerTackleAnimator.h"
#include "EnemyTackleAnimator.h"
#include "PlayerGrowlAnimator.h"
#include "StatStageChangeAnimator.h"

AAnimatorGenerator::AAnimatorGenerator() 
{
}

AAnimatorGenerator::~AAnimatorGenerator() 
{
}

AAnimator* AAnimatorGenerator::GenerateMoveAnimator(UBattler* _Attacker, EPokemonMove _MoveId)
{
	if (true == _Attacker->IsPlayer())
	{
		switch (_MoveId)
		{
		case EPokemonMove::LeechSeed:
		case EPokemonMove::Growl:
			return GetWorld()->SpawnActor <APlayerGrowlAnimator>();
		case EPokemonMove::Tackle:
		default:
			return GetWorld()->SpawnActor<APlayerTackleAnimator>();
		}
	}
	else
	{
		switch (_MoveId)
		{
		case EPokemonMove::Tackle:
		default:
			return GetWorld()->SpawnActor<AEnemyTackleAnimator>();
		}
	}

	return GetWorld()->SpawnActor<APlayerTackleAnimator>();
}

AAnimator* AAnimatorGenerator::GenerateStatStageEffectAnimator(UBattler* _Target, bool _IsStatDown)
{
	EPokemonId PokemonId = _Target->CurPokemonReadonly()->GetPokedexNo();
	AStatStageChangeAnimator* Animator = GetWorld()->SpawnActor<AStatStageChangeAnimator>();
	Animator->SetIsStatDown(_IsStatDown);
	Animator->SetPokemonId(PokemonId);
	if (true == _Target->IsPlayer())
	{
		Animator->SetIsPlayer(true);
	}
	else
	{
		Animator->SetIsPlayer(false);
	}

	return Animator;
}
