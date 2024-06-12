#pragma once
#include "Animator.h"

class APlayerCharacterTackleAnimator : public AAnimator
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
	APlayerCharacterTackleAnimator();
	~APlayerCharacterTackleAnimator();

	// delete Function
	APlayerCharacterTackleAnimator(const APlayerCharacterTackleAnimator& _Other) = delete;
	APlayerCharacterTackleAnimator(APlayerCharacterTackleAnimator&& _Other) noexcept = delete;
	APlayerCharacterTackleAnimator& operator=(const APlayerCharacterTackleAnimator& _Other) = delete;
	APlayerCharacterTackleAnimator& operator=(APlayerCharacterTackleAnimator&& _Other) noexcept = delete;

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
	void ProcessPlayerMoveRight();
	void ProcessShowTackleEffect();
	void ProcessPlayerMoveLeft();
	void ProcessHideTackleEffect();
	void ProcessWaitBlinkEffectEnd();

	// 데이터
	FVector PlayerRightPos;
	const float MoveTime = 0.1f;
	float Timer = 0.0f;
	const float DamageSoundWaitTime = 0.4f;
	bool IsDamageSoundPlayed = false;
};

