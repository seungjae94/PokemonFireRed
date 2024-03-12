#pragma once
#include "Animator.h"

class AEnemyTackleAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		EnemyMoveLeft,
		ShowTackleEffect,
		EnemyMoveRight,
		HideTackleEffect,
		WaitBlinkEffectEnd,
		End,
	};
public:
	// constructor destructor
	AEnemyTackleAnimator();
	~AEnemyTackleAnimator();

	// delete Function
	AEnemyTackleAnimator(const AEnemyTackleAnimator& _Other) = delete;
	AEnemyTackleAnimator(AEnemyTackleAnimator&& _Other) noexcept = delete;
	AEnemyTackleAnimator& operator=(const AEnemyTackleAnimator& _Other) = delete;
	AEnemyTackleAnimator& operator=(AEnemyTackleAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* TackleEffect = nullptr;

	// 상태
	EState State = EState::None;
	void ProcessEnemyMoveLeft();
	void ProcessShowTackleEffect();
	void ProcessEnemyMoveRight();
	void ProcessHideTackleEffect();
	void ProcessWaitBlinkEffectEnd();

	// 데이터
	FVector EnemyLeftPos;
	const float MoveTime = 0.1f;
	const float BeforeBlinkEffectTime = 0.25f;
	float Timer = 0.0f;
};

