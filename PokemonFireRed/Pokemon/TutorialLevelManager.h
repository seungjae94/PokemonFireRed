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
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetArrowPos(const FVector& _Pos);

	AEventTrigger* LevelEndTrigger = nullptr;

	int PageIndex = 0;
	bool IsEnd = false;
	std::string GetPageName();

	FVector ArrowMoveRange = FVector::Up * 20.0f;
	FVector ArrowDownPos;
	bool IsArrowMoveUpward = true;
	float MaxArrowValue = 0.33f;
	float ArrowValue = 0.0f;

	UImageRenderer* Renderer = nullptr;
	UImageRenderer* ArrowRenderer = nullptr;
	UImageRenderer* PikachuRenderer = nullptr;

	
};

