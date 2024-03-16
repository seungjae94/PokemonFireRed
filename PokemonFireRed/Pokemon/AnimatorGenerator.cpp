#include "AnimatorGenerator.h"
#include "Battler.h"
#include "Pokemon.h"
#include "PlayerTackleAnimator.h"
#include "EnemyTackleAnimator.h"
#include "GrowlAnimator.h"
#include "StatStageChangeAnimator.h"

AAnimatorGenerator::AAnimatorGenerator()
{
}

AAnimatorGenerator::~AAnimatorGenerator()
{
}

AAnimator* AAnimatorGenerator::GenerateMoveAnimator(UBattler* _Attacker, EPokemonMove _MoveId, ETypeVs _TypeVs)
{
	if (true == _Attacker->IsPlayer())
	{
		switch (_MoveId)
		{
		case EPokemonMove::LeechSeed:
		case EPokemonMove::Growl:
		{
			AGrowlAnimator* Animator = GetWorld()->SpawnActor<AGrowlAnimator>();
			Animator->SetIsPlayer(true);
			return Animator;
		}
		case EPokemonMove::Tackle:
		default:
		{
			APlayerTackleAnimator* Animator = GetWorld()->SpawnActor<APlayerTackleAnimator>();
			Animator->SetTypeVs(_TypeVs);
			return Animator;
		}
		}
	}
	else
	{
		switch (_MoveId)
		{
		case EPokemonMove::Harden:
		case EPokemonMove::DefenseCurl:
		case EPokemonMove::StringShot:
		case EPokemonMove::TailWhip:
		case EPokemonMove::SandAttack:
		case EPokemonMove::Growl:
		{
			AGrowlAnimator* Animator = GetWorld()->SpawnActor<AGrowlAnimator>();
			Animator->SetIsPlayer(false);
			return Animator;
		}
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
