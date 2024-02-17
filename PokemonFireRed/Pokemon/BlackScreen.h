#pragma once
#include "FadeScreen.h"

class UEventManager;

class ABlackScreen : public AFadeScreen
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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void FadeInTick(float _DeltaTime);
	void FadeOutTick(float _DeltaTime);
};

