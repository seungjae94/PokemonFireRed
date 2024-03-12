#include "AnimatorGenerator.h"
#include "Battler.h"
#include "Pokemon.h"
#include "PlayerTackleAnimator.h"
#include "EnemyTackleAnimator.h"
#include "PlayerGrowlAnimator.h"

AAnimatorGenerator::AAnimatorGenerator() 
{
}

AAnimatorGenerator::~AAnimatorGenerator() 
{
}

AAnimator* AAnimatorGenerator::Generate(UBattler* _Attacker, EPokemonMove _MoveId)
{
	if (true == _Attacker->IsPlayer())
	{
		switch (_MoveId)
		{
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
