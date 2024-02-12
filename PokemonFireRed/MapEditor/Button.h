#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AButton : public AActor
{
public:
	// constructor destructor
	AButton();
	~AButton();

	// delete Function
	AButton(const AButton& _Other) = delete;
	AButton(AButton&& _Other) noexcept = delete;
	AButton& operator=(const AButton& _Other) = delete;
	AButton& operator=(AButton&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* ButtonBackgroundRenderer = nullptr;
	UImageRenderer* ButtonHighLightRenderer = nullptr;
	UImageRenderer* IconRenderer = nullptr;
};

