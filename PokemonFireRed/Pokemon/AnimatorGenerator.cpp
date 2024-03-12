#include "AnimatorGenerator.h"
#include "Battler.h"
#include "Pokemon.h"
#include "PlayerTackleAnimator.h"
#include "EnemyTackleAnimator.h"
#include "PlayerGrowlAnimator.h"
#include "EnemyStatStageChangeAnimator.h"

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

AAnimator* AAnimatorGenerator::Generate(UBattler* _Attacker, bool _IsStatDown)
{
	EPokemonId PokemonId = _Attacker->CurPokemonReadonly()->GetPokedexNo();

	if (true == _Attacker->IsPlayer())
	{
		AEnemyStatStageChangeAnimator* Animator = GetWorld()->SpawnActor<AEnemyStatStageChangeAnimator>();
		Animator->SetIsStatDown(_IsStatDown);
		Animator->SetPokemonId(PokemonId);
		return Animator;
	}
	else
	{
		// 임시로 적 스탯 감소 이펙트를 넣었음
		AEnemyStatStageChangeAnimator* Animator = GetWorld()->SpawnActor<AEnemyStatStageChangeAnimator>();
		Animator->SetIsStatDown(_IsStatDown);
		Animator->SetPokemonId(PokemonId);
		return Animator;
	}

	return nullptr;
}
