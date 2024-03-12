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
		MsgBoxAssert("��Ʋ ������ �ƴ� ������ BattleStateMachine�� �����߽��ϴ�.");
		return;
	}

	BattleCanvas = BattleLevel->Canvas;
	BlinkEffectAnimator = BattleLevel->BlinkEffectAnimator;
	ShakeEffectAnimator = BattleLevel->ShakeEffectAnimator;
	PlayerInitPos = BattleCanvas->PlayerPokemonImageInitPos;
	EnemyInitPos = BattleCanvas->EnemyPokemonImageInitPos;
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

