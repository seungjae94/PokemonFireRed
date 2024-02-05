#pragma once
#include <EngineCore/Actor.h>

class UImageRenderer;

class UTitleLevelIntroImage : public AActor
{
public:
	// constructor destructor
	UTitleLevelIntroImage();
	~UTitleLevelIntroImage();

	// delete Function
	UTitleLevelIntroImage(const UTitleLevelIntroImage& _Other) = delete;
	UTitleLevelIntroImage(UTitleLevelIntroImage&& _Other) noexcept = delete;
	UTitleLevelIntroImage& operator=(const UTitleLevelIntroImage& _Other) = delete;
	UTitleLevelIntroImage& operator=(UTitleLevelIntroImage&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	UImageRenderer* Renderer = nullptr;
};

