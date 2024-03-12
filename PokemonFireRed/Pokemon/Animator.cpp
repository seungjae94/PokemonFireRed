#include "Animator.h"
#include "BattleLevel.h"

AAnimator::AAnimator() 
{
}

AAnimator::~AAnimator() 
{
}

void AAnimator::Start()
{
	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(GetWorld());

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("배틀 레벨이 아닌 곳에서 BattleStateMachine을 생성했습니다.");
		return;
	}

	BattleCanvas = BattleLevel->Canvas;
	PlayerInitPos = BattleCanvas->PlayerPokemonImage->GetRelativePosition();
	EnemyInitPos = BattleCanvas->EnemyPokemonImage->GetRelativePosition();
}

bool AAnimator::IsEnd()
{
	return false;
}

AImageElement* AAnimator::GetPlayer()
{
	return BattleCanvas->PlayerPokemonImage;
}

AImageElement* AAnimator::GetEnemy()
{
	return BattleCanvas->EnemyPokemonImage;
}

