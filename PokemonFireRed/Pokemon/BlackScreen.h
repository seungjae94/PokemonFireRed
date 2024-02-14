#pragma once
#include "UIElement.h"

class ABlackScreen : public AUIElement
{
public:
	// constructor destructor
	ABlackScreen();
	~ABlackScreen();

	// delete Function
	ABlackScreen(const ABlackScreen& _Other) = delete;
	ABlackScreen(ABlackScreen&& _Other) noexcept = delete;
	ABlackScreen& operator=(const ABlackScreen& _Other) = delete;
	ABlackScreen& operator=(ABlackScreen&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;
};

