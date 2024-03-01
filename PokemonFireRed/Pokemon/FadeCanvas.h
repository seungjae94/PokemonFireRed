#pragma once
#include "Canvas.h"

enum class EFadeCanvasState
{
	Wait,
	BlackFadeIn,
	BlackFadeOut,
	WhiteFadeIn,
	WhiteFadeOut,
	VCurtainOpen,
	VCurtainClose,
	HCurtainOpen,
	HCurtainClose
};

class AFadeCanvas : public ACanvas
{
public:
	// constructor destructor
	AFadeCanvas();
	~AFadeCanvas();

	// delete Function
	AFadeCanvas(const AFadeCanvas& _Other) = delete;
	AFadeCanvas(AFadeCanvas&& _Other) noexcept = delete;
	AFadeCanvas& operator=(const AFadeCanvas& _Other) = delete;
	AFadeCanvas& operator=(AFadeCanvas&& _Other) noexcept = delete;

	void Clear();
	void FadeInBlack(float _Time);
	void FadeOutBlack(float _Time);
	void FadeInWhite(float _Time);
	void FadeOutWhite(float _Time);
	void OpenVCurtain(float _Time);
	void OpenHCurtain(float _Time);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	EFadeCanvasState State = EFadeCanvasState::Wait;
	float Timer = 0.0f;
	float FadeTime = 1.0f;

	AImageElement* BlackScreen = nullptr;
	AImageElement* WhiteScreen = nullptr;
	AImageElement* VCurtainTop = nullptr;
	AImageElement* VCurtainBot = nullptr;
	AImageElement* HCurtainLeft = nullptr;
	AImageElement* HCurtainRight = nullptr;

	FVector VCurtainTopHidePos;
	FVector VCurtainTopShowPos;
	FVector VCurtainBotHidePos;
	FVector VCurtainBotShowPos;
	FVector HCurtainLeftHidePos;
	FVector HCurtainLeftShowPos;
	FVector HCurtainRightHidePos;
	FVector HCurtainRightShowPos;

	// 처리 함수
	void ProcessBlackFadeIn(float _DeltaTime);
	void ProcessBlackFadeOut(float _DeltaTime);
	void ProcessWhiteFadeIn(float _DeltaTime);
	void ProcessWhiteFadeOut(float _DeltaTime);
	void ProcessVCurtainOpen(float _DeltaTime);
	void ProcessHCurtainOpen(float _DeltaTime);
};

