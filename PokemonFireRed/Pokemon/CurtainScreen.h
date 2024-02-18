#pragma once
#include "FadeScreen.h"

class ACurtainScreen : public AFadeScreen
{
public:
	// constructor destructor
	ACurtainScreen();
	~ACurtainScreen();

	// delete Function
	ACurtainScreen(const ACurtainScreen& _Other) = delete;
	ACurtainScreen(ACurtainScreen&& _Other) noexcept = delete;
	ACurtainScreen& operator=(const ACurtainScreen& _Other) = delete;
	ACurtainScreen& operator=(ACurtainScreen&& _Other) noexcept = delete;

	void FadeIn(float _FadeTime) override;
	void FadeOut(float _FadeTime) override;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void Sync(AUIElement* _Other) override;
private:
	void FadeInTick(float _DeltaTime);
	void FadeOutTick(float _DeltaTime);

	UImageRenderer* LeftRenderer = nullptr;
	UImageRenderer* RightRenderer = nullptr;
};

