#pragma once
#include "FadeScreen.h"

class AWhiteScreen : public AFadeScreen
{
public:
	// constructor destructor
	AWhiteScreen();
	~AWhiteScreen();

	// delete Function
	AWhiteScreen(const AWhiteScreen& _Other) = delete;
	AWhiteScreen(AWhiteScreen&& _Other) noexcept = delete;
	AWhiteScreen& operator=(const AWhiteScreen& _Other) = delete;
	AWhiteScreen& operator=(AWhiteScreen&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void FadeInTick(float _DeltaTime);
	void FadeOutTick(float _DeltaTime);
};

