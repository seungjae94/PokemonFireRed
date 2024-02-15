#pragma once
#include "UIElement.h"

class UEventManager;

class ABlackScreen : public AUIElement
{
	friend UEventManager;
public:
	// constructor destructor
	ABlackScreen();
	~ABlackScreen();

	// delete Function
	ABlackScreen(const ABlackScreen& _Other) = delete;
	ABlackScreen(ABlackScreen&& _Other) noexcept = delete;
	ABlackScreen& operator=(const ABlackScreen& _Other) = delete;
	ABlackScreen& operator=(ABlackScreen&& _Other) noexcept = delete;

	void SetAlpha(float _Alpha)
	{
		Renderer->SetAlpha(_Alpha);
	}

	void SetActiveRenderer(bool _Active)
	{
		Renderer->SetActive(_Active);
	}


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Renderer = nullptr;
};

