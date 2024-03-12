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

	virtual void Start();
	
	virtual bool IsEnd();

protected:
	ABattleCanvas* BattleCanvas = nullptr;

	FVector PlayerInitPos;
	FVector EnemyInitPos;

	AImageElement* GetPlayer();
	AImageElement* GetEnemy();
private:

};

