#pragma once
#include "Animator.h"

class APlayerGrowlAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		CheckMore,
		ShowGrowlEffect,
		WaitShakeEffectEnd,
		End
	};
public:
	// constructor destructor
	APlayerGrowlAnimator();
	~APlayerGrowlAnimator();

	// delete Function
	APlayerGrowlAnimator(const APlayerGrowlAnimator& _Other) = delete;
	APlayerGrowlAnimator(APlayerGrowlAnimator&& _Other) noexcept = delete;
	APlayerGrowlAnimator& operator=(const APlayerGrowlAnimator& _Other) = delete;
	APlayerGrowlAnimator& operator=(APlayerGrowlAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* UpEffect = nullptr;
	AImageElement* MidEffect = nullptr;
	AImageElement* DownEffect = nullptr;


	// 상태
	EState State = EState::None;
	void ProcessCheckMore();
	void ProcessShowGrowlEffect();
	void ProcessWaitShakeEffectEnd();

	// 데이터
	FVector UpEffectInitPos;
	FVector MidEffectInitPos;
	FVector DownEffectInitPos;
	FVector UpEffectTargetPos;
	FVector MidEffectTargetPos;
	FVector DownEffectTargetPos;

	const float MoveTime = 0.35f;
	float Timer = 0.0f;
	const int MaxLoopCount = 2;
	int LoopCount = 2;

};

