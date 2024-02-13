#pragma once
#include <EngineCore/Actor.h>


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

	bool IsClicked(const FVector& _MousePos);
	void SetIcon(std::string_view _IconName);
	void ToggleHighlight();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* ButtonBackgroundRenderer = nullptr;
	UImageRenderer* ButtonHighLightRenderer = nullptr;
	UImageRenderer* IconRenderer = nullptr;
};

