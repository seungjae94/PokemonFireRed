#pragma once
#include "Animator.h"

class AEnemyStatStageChangeAnimator : public AAnimator
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
	AEnemyStatStageChangeAnimator();
	~AEnemyStatStageChangeAnimator();

	// delete Function
	AEnemyStatStageChangeAnimator(const AEnemyStatStageChangeAnimator& _Other) = delete;
	AEnemyStatStageChangeAnimator(AEnemyStatStageChangeAnimator&& _Other) noexcept = delete;
	AEnemyStatStageChangeAnimator& operator=(const AEnemyStatStageChangeAnimator& _Other) = delete;
	AEnemyStatStageChangeAnimator& operator=(AEnemyStatStageChangeAnimator&& _Other) noexcept = delete;

	void Start() override;
	bool IsEnd() override;

	void SetIsStatDown(bool _IsStatDown);
	void SetPokemonId(EPokemonId _PokemonId);

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool IsStatDown = false;
	EPokemonId PokemonId = EPokemonId::None;

	std::vector<AImageElement*> StatImages;

	EState State = EState::None;
	const int MaxWorkCount = 16;
	int WorkCount = 16;
	int ImageIndex = 0;
	const float ImageChangeInterval = 0.05f;
	float Timer = 0.0f;

	// »óÅÂ Æ½ ÇÔ¼ö
	void ProcessCheckWork();
	void ProcessWork();
};

