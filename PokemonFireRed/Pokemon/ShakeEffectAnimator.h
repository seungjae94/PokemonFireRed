#pragma once
#include "Animator.h"

class AShakeEffectAnimator : public AAnimator
{
public:
	// constructor destructor
	AShakeEffectAnimator();
	~AShakeEffectAnimator();

	// delete Function
	AShakeEffectAnimator(const AShakeEffectAnimator& _Other) = delete;
	AShakeEffectAnimator(AShakeEffectAnimator&& _Other) noexcept = delete;
	AShakeEffectAnimator& operator=(const AShakeEffectAnimator& _Other) = delete;
	AShakeEffectAnimator& operator=(AShakeEffectAnimator&& _Other) noexcept = delete;

protected:

private:

};

