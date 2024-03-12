#pragma once

class AEnemyGrowlAnimator
{
public:
	// constructor destructor
	AEnemyGrowlAnimator();
	~AEnemyGrowlAnimator();

	// delete Function
	AEnemyGrowlAnimator(const AEnemyGrowlAnimator& _Other) = delete;
	AEnemyGrowlAnimator(AEnemyGrowlAnimator&& _Other) noexcept = delete;
	AEnemyGrowlAnimator& operator=(const AEnemyGrowlAnimator& _Other) = delete;
	AEnemyGrowlAnimator& operator=(AEnemyGrowlAnimator&& _Other) noexcept = delete;

protected:

private:

};

