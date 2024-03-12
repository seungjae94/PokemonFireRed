#pragma once
#include "Animator.h"

class ABlinkEffectAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		Wait,
		CheckMore,
		Hide,
		Show,
		End
	};
public:
	// constructor destructor
	ABlinkEffectAnimator();
	~ABlinkEffectAnimator();

	// delete Function
	ABlinkEffectAnimator(const ABlinkEffectAnimator& _Other) = delete;
	ABlinkEffectAnimator(ABlinkEffectAnimator&& _Other) noexcept = delete;
	ABlinkEffectAnimator& operator=(const ABlinkEffectAnimator& _Other) = delete;
	ABlinkEffectAnimator& operator=(ABlinkEffectAnimator&& _Other) noexcept = delete;

	void Start(bool _IsPlayer, float _WaitTime = 0.0f);
	bool IsEnd() override;

protected:

private:
	void Start() override {};

	EState State = EState::None;
	bool IsPlayer = true;
	float WaitTime = 0.0f;
	float Timer = 0.0f;
	const float StateTime = 0.1f;
	const int MaxLoopCount = 4;
	int LoopCount = 4;

	// ╩Себ ф╫
	void Tick(float _DeltaTime) override;
	void ProcessWait();
	void ProcessCheckMore();
	void ProcessHide();
	void ProcessShow();
};

