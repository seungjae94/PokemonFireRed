#pragma once
#include "Animator.h"

class AShakeEffectAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		MoveOrigin2Right,
		CheckMore,
		MoveRight2Left,
		MoveLeft2Right,
		FinalMoveRight2Left,
		MoveLeft2Origin,
		End
	};
public:
	// constructor destructor
	AShakeEffectAnimator();
	~AShakeEffectAnimator();

	// delete Function
	AShakeEffectAnimator(const AShakeEffectAnimator& _Other) = delete;
	AShakeEffectAnimator(AShakeEffectAnimator&& _Other) noexcept = delete;
	AShakeEffectAnimator& operator=(const AShakeEffectAnimator& _Other) = delete;
	AShakeEffectAnimator& operator=(AShakeEffectAnimator&& _Other) noexcept = delete;

	void Start(bool _IsPlayer);
	bool IsEnd() override;

protected:

private:
	// ©Л аб ©Л аб ©Л аб ©Л аб ©Ь

	void Start() override {};

	EState State = EState::None;
	bool IsPlayer = true;
	float Timer = 0.0f;
	const float StateTime = 0.05f;
	const int MaxLoopCount = 3;
	int LoopCount = 3;
	FVector* InitPos = nullptr;
	FVector RightPos;
	FVector LeftPos;

	// ╩Себ ф╫
	void Tick(float _DeltaTime) override;
	void ProcessMoveOrigin2Right();
	void ProcessCheckMore();
	void ProcessMoveRight2Left();
	void ProcessMoveLeft2Right();
	void ProcessFinalMoveRight2Left();
	void ProcessMoveLeft2Origin();
};

