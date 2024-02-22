#pragma once
#include <EngineCore/Actor.h>

class AEventTrigger;

class ATutorialLevelManager : public AActor
{
public:
	// constructor destructor
	ATutorialLevelManager();
	~ATutorialLevelManager();

	// delete Function
	ATutorialLevelManager(const ATutorialLevelManager& _Other) = delete;
	ATutorialLevelManager(ATutorialLevelManager&& _Other) noexcept = delete;
	ATutorialLevelManager& operator=(const ATutorialLevelManager& _Other) = delete;
	ATutorialLevelManager& operator=(ATutorialLevelManager&& _Other) noexcept = delete;

protected:

private:
	enum class EFadeState
	{
		None,
		FadeOut,
		FadeIn,
		End
	};

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetArrowPos(const FVector& _Pos);

	int PageIndex = 0;
	std::string GetPageName();

	// 화살표
	FVector ArrowMoveRange = FVector::Up * 20.0f;
	FVector ArrowDownPos;
	bool IsArrowMoveUpward = true;
	float MaxArrowValue = 0.3f;
	float ArrowValue = 0.0f;

	// 피카츄
	std::string PikachuImageName = "StandEyeOpenEarClose.png";
	bool IsStand = true;
	bool IsEarClose = true;
	bool IsEyeOpen = true;

	float CalcTime = 1.0f / 60.0f;
	float CurCalcTime = CalcTime;
	int CalcCount = 0;
	int PrevCalcCount = 0;

	const int EyeChangeDivisor = 558;
	const int EarChangeDivisor = 674;
	const std::vector<int> EyeChangeRemainders = { 30, 39, 48, 57, 236, 245, 254, 263 };
	const std::vector<int> EarChangeRemainders = { 60, 72, 84, 96, 280, 292, 304, 316 };

	// 렌더러
	UImageRenderer* Renderer = nullptr;
	UImageRenderer* ArrowRenderer = nullptr;
	UImageRenderer* PikachuRenderer = nullptr;

	// 페이지 변경 페이드 효과
	EFadeState FadingState = EFadeState::None;
	float FadeOutTime = 0.5f;
	float FadeInTime = 0.5f;
	float UIChangeWaitTime = 0.2f;
	float CurFadeOutTime = FadeOutTime;
	float CurFadeInTime = FadeInTime;
	bool UiChanged = false;
};

