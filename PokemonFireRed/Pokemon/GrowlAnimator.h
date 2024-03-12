#pragma once
#include "Animator.h"

class AGrowlAnimator : public AAnimator
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
	AGrowlAnimator();
	~AGrowlAnimator();

	// delete Function
	AGrowlAnimator(const AGrowlAnimator& _Other) = delete;
	AGrowlAnimator(AGrowlAnimator&& _Other) noexcept = delete;
	AGrowlAnimator& operator=(const AGrowlAnimator& _Other) = delete;
	AGrowlAnimator& operator=(AGrowlAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

	void SetIsPlayer(bool _IsPlayer);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* PlayerUpEffect = nullptr;
	AImageElement* PlayerMidEffect = nullptr;
	AImageElement* PlayerDownEffect = nullptr;
	AImageElement* EnemyUpEffect = nullptr;
	AImageElement* EnemyMidEffect = nullptr;
	AImageElement* EnemyDownEffect = nullptr;
	AImageElement* UpEffect = nullptr;
	AImageElement* MidEffect = nullptr;
	AImageElement* DownEffect = nullptr;

	// 상태
	EState State = EState::None;
	void ProcessCheckMore();
	void ProcessShowGrowlEffect();
	void ProcessWaitShakeEffectEnd();

	// 데이터
	bool IsPlayer = true;
	FVector PlayerUpEffectInitPos;
	FVector PlayerMidEffectInitPos;
	FVector PlayerDownEffectInitPos;
	FVector PlayerUpEffectTargetPos;
	FVector PlayerMidEffectTargetPos;
	FVector PlayerDownEffectTargetPos;
	FVector EnemyUpEffectInitPos;
	FVector EnemyMidEffectInitPos;
	FVector EnemyDownEffectInitPos;
	FVector EnemyUpEffectTargetPos;
	FVector EnemyMidEffectTargetPos;
	FVector EnemyDownEffectTargetPos;
	FVector* UpEffectInitPos = nullptr;
	FVector* MidEffectInitPos = nullptr;
	FVector* DownEffectInitPos = nullptr;
	FVector* UpEffectTargetPos = nullptr;
	FVector* MidEffectTargetPos = nullptr;
	FVector* DownEffectTargetPos = nullptr;

	const float MoveTime = 0.35f;
	float Timer = 0.0f;
	const int MaxLoopCount = 2;
	int LoopCount = 2;

};

