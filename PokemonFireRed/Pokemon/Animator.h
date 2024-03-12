#pragma once
#include "Canvas.h"
#include "BattleCanvas.h"

class AAnimator : public ACanvas
{
public:
	// constructor destructor
	AAnimator();
	~AAnimator();

	// delete Function
	AAnimator(const AAnimator& _Other) = delete;
	AAnimator(AAnimator&& _Other) noexcept = delete;
	AAnimator& operator=(const AAnimator& _Other) = delete;
	AAnimator& operator=(AAnimator&& _Other) noexcept = delete;

	virtual void Start(bool _IsTargetPlayer) {
		IsTargetPlayer = _IsTargetPlayer;
		PlayerInitPos = BattleCanvas->PlayerPokemonImage->GetRelativePosition();
		EnemyInitPos = BattleCanvas->EnemyPokemonImage->GetRelativePosition();
	}
	
	virtual bool IsEnd() { 
		return true; 
	};

protected:
	bool IsTargetPlayer = true;
	ABattleCanvas* BattleCanvas = nullptr;

	FVector PlayerInitPos;
	FVector EnemyInitPos;

	AImageElement* GetPlayer();
	AImageElement* GetEnemy();
private:

};

