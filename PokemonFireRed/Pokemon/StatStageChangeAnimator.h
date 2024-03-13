#pragma once
#include "Animator.h"

class AStatStageChangeAnimator : public AAnimator
{
private:
	enum class EState
	{
		None,
		CheckWork,
		Work,
		End
	};
public:
	// constructor destructor
	AStatStageChangeAnimator();
	~AStatStageChangeAnimator();

	// delete Function
	AStatStageChangeAnimator(const AStatStageChangeAnimator& _Other) = delete;
	AStatStageChangeAnimator(AStatStageChangeAnimator&& _Other) noexcept = delete;
	AStatStageChangeAnimator& operator=(const AStatStageChangeAnimator& _Other) = delete;
	AStatStageChangeAnimator& operator=(AStatStageChangeAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

	void SetIsPlayer(bool _IsPlayer);
	void SetIsStatDown(bool _IsStatDown);
	void SetPokemonId(EPokemonId _PokemonId);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool IsPlayer = false;
	bool IsStatDown = false;
	EPokemonId PokemonId = EPokemonId::None;

	std::vector<AImageElement*> StatImages;

	EState State = EState::None;
	const int MaxWorkCount = 16;
	int WorkCount = 16;
	int ImageIndex = 0;
	const float ImageChangeInterval = 0.05f;
	float Timer = 0.0f;
	
	const float MaxAlpha = 0.5f;
	const float AlphaTime = MaxWorkCount * ImageChangeInterval;
	const float FadeTime = 0.2f;
	float AlphaTimer = 0.0f;


	// 상태 틱 함수
	void ProcessCheckWork();
	void ProcessWork();

	// 유틸 함수
	int GetEffectiveImageIndex() const;
	float GetAlpha() const;
};

