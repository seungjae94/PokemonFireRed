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

	void SetTypeVs(ETypeVs _TypeVs);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* TackleEffect = nullptr;

	// 상태
	EState State = EState::None;
	ETypeVs TypeVs = ETypeVs::NormallyEffective;
	void ProcessEnemyMoveLeft();
	void ProcessShowTackleEffect();
	void ProcessEnemyMoveRight();
	void ProcessHideTackleEffect();
	void ProcessWaitBlinkEffectEnd();

	// 데이터
	FVector EnemyLeftPos;
	const float MoveTime = 0.1f;
	float Timer = 0.0f;

	const float DamageSoundWaitTime = 0.4f;
	bool IsDamageSoundPlayed = false;
};

