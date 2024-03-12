#pragma once
#include "Animator.h"

class APlayerTackleAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		PlayerMoveRight,
		ShowTackleEffect,
		PlayerMoveLeft,
		HideTackleEffect,
		WaitBlinkEffectEnd,
		End,
	};
public:
	// constructor destructor
	APlayerTackleAnimator();
	~APlayerTackleAnimator();

	// delete Function
	APlayerTackleAnimator(const APlayerTackleAnimator& _Other) = delete;
	APlayerTackleAnimator(APlayerTackleAnimator&& _Other) noexcept = delete;
	APlayerTackleAnimator& operator=(const APlayerTackleAnimator& _Other) = delete;
	APlayerTackleAnimator& operator=(APlayerTackleAnimator&& _Other) noexcept = delete;

	void Start(bool _IsTargetPlayer) override;
	bool IsEnd() override;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	AImageElement* TackleEffect = nullptr;
	
	// 상태
	EState State = EState::None;
	void ProcessPlayerMoveRight();
	void ProcessShowTackleEffect();
	void ProcessPlayerMoveLeft();
	void ProcessHideTackleEffect();
	void ProcessWaitBlinkEffectEnd();

	// 데이터
	const FVector PlayerRightPos = PlayerInitPos + UPokemonUtil::PixelVector(10, 0);
	const float MoveTime = 0.25f;
	float Timer = 0.0f;
};

