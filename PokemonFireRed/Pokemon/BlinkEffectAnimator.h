#pragma once

class BlinkEffectAnimator
{
public:
	// constructor destructor
	BlinkEffectAnimator();
	~BlinkEffectAnimator();

	// delete Function
	BlinkEffectAnimator(const BlinkEffectAnimator& _Other) = delete;
	BlinkEffectAnimator(BlinkEffectAnimator&& _Other) noexcept = delete;
	BlinkEffectAnimator& operator=(const BlinkEffectAnimator& _Other) = delete;
	BlinkEffectAnimator& operator=(BlinkEffectAnimator&& _Other) noexcept = delete;

protected:

private:

};

