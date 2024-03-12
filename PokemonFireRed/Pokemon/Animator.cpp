#include "Animator.h"

AAnimator::AAnimator() 
{
}

AAnimator::~AAnimator() 
{
}

AImageElement* AAnimator::GetPlayer()
{
	return BattleCanvas->PlayerPokemonImage;
}

AImageElement* AAnimator::GetEnemy()
{
	return BattleCanvas->EnemyPokemonImage;
}

