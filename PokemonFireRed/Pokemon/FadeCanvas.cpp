#include "FadeCanvas.h"

AFadeCanvas::AFadeCanvas() 
{
}

AFadeCanvas::~AFadeCanvas() 
{
}

void AFadeCanvas::BeginPlay()
{
	BlackScreen = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, 0, 0);
	BlackScreen->SetImage(RN::BlackScreen);
	BlackScreen->SetAlpha(0.0f);

	WhiteScreen = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, 0, 0);
	WhiteScreen->SetImage(RN::WhiteScreen);
	WhiteScreen->SetAlpha(0.0f);

	VCurtainTop = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, 0, -Global::PixelScreenY);
	VCurtainTop->SetImage(RN::BlackScreen);
	VCurtainTopHidePos = VCurtainTop->GetRelativePosition();
	VCurtainTopShowPos = VCurtainTopHidePos + FVector::Down * Global::HalfScreenY;

	VCurtainBot = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, 0, Global::PixelScreenY);
	VCurtainBot->SetImage(RN::BlackScreen);
	VCurtainBotHidePos = VCurtainBot->GetRelativePosition();
	VCurtainBotShowPos = VCurtainBotHidePos + FVector::Up * Global::HalfScreenY;

	HCurtainLeft = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, -Global::PixelScreenX, 0);
	HCurtainLeft->SetImage(RN::BlackScreen);
	HCurtainLeftHidePos = HCurtainLeft->GetRelativePosition();
	HCurtainLeftShowPos = HCurtainLeftHidePos + FVector::Right * Global::HalfScreenX;

	HCurtainRight = CreateImageElement(this, ERenderingOrder::Fade, EPivotType::LeftTop, Global::PixelScreenX, 0);
	HCurtainRight->SetImage(RN::BlackScreen);
	HCurtainRightHidePos = HCurtainRight->GetRelativePosition();
	HCurtainRightShowPos = HCurtainRightHidePos + FVector::Left * Global::HalfScreenX;
}

void AFadeCanvas::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case EFadeCanvasState::Wait:
		break;
	case EFadeCanvasState::BlackFadeIn:
		ProcessBlackFadeIn(_DeltaTime);
		break;
	case EFadeCanvasState::BlackFadeOut:
		ProcessBlackFadeOut(_DeltaTime);
		break;
	case EFadeCanvasState::WhiteFadeIn:
		ProcessWhiteFadeIn(_DeltaTime);
		break;
	case EFadeCanvasState::WhiteFadeOut:
		ProcessWhiteFadeOut(_DeltaTime);
		break;
	case EFadeCanvasState::VCurtainOpen:
		ProcessVCurtainOpen(_DeltaTime);
		break;
	case EFadeCanvasState::VCurtainClose:
		break;
	case EFadeCanvasState::HCurtainOpen:
		ProcessHCurtainOpen(_DeltaTime);
		break;
	case EFadeCanvasState::HCurtainClose:
		break;
	default:
		break;
	}
}

void AFadeCanvas::Clear()
{
	EFadeCanvasState State = EFadeCanvasState::Wait;
	BlackScreen->SetAlpha(0.0f);
	WhiteScreen->SetAlpha(0.0f);
	VCurtainTop->SetRelativePosition(VCurtainTopHidePos);
	VCurtainBot->SetRelativePosition(VCurtainBotHidePos);
	HCurtainLeft->SetRelativePosition(HCurtainLeftHidePos);
	HCurtainRight->SetRelativePosition(HCurtainRightHidePos);
}

void AFadeCanvas::FadeInBlack(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::BlackFadeIn;
	BlackScreen->SetAlpha(0.0f);
	FadeTime = _Time;
	Timer = _Time;
}

void AFadeCanvas::FadeOutBlack(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::BlackFadeOut;
	BlackScreen->SetAlpha(1.0f);
	FadeTime = _Time;
	Timer = _Time;
}

void AFadeCanvas::FadeInWhite(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::WhiteFadeIn;
	WhiteScreen->SetAlpha(0.0f);
	FadeTime = _Time;
	Timer = _Time;
}

void AFadeCanvas::FadeOutWhite(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::WhiteFadeOut;
	WhiteScreen->SetAlpha(1.0f);
	FadeTime = _Time;
	Timer = _Time;
}

void AFadeCanvas::OpenVCurtain(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::VCurtainOpen;
	VCurtainTop->SetRelativePosition(VCurtainTopShowPos);
	VCurtainBot->SetRelativePosition(VCurtainBotShowPos);
	FadeTime = _Time;
	Timer = _Time;
}

void AFadeCanvas::OpenHCurtain(float _Time)
{
	Clear();
	SetActive(true);

	State = EFadeCanvasState::HCurtainOpen;
	HCurtainLeft->SetRelativePosition(HCurtainLeftShowPos);
	HCurtainRight->SetRelativePosition(HCurtainRightShowPos);
	FadeTime = _Time;
	Timer = _Time;
}


void AFadeCanvas::ProcessBlackFadeIn(float _DeltaTime)
{
	BlackScreen->SetAlpha(Timer / FadeTime);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
		SetActive(false);
	}
}

void AFadeCanvas::ProcessBlackFadeOut(float _DeltaTime)
{
	BlackScreen->SetAlpha(1.0f - Timer / FadeTime);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
	}
}

void AFadeCanvas::ProcessWhiteFadeIn(float _DeltaTime)
{
	WhiteScreen->SetAlpha(Timer / FadeTime);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
		SetActive(false);
	}
}

void AFadeCanvas::ProcessWhiteFadeOut(float _DeltaTime)
{
	WhiteScreen->SetAlpha(1.0f - Timer / FadeTime);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
	}
}

void AFadeCanvas::ProcessVCurtainOpen(float _DeltaTime)
{
	FVector TopPos = UPokemonMath::Lerp(VCurtainTopHidePos, VCurtainTopShowPos, Timer / FadeTime);
	FVector BotPos = UPokemonMath::Lerp(VCurtainBotHidePos, VCurtainBotShowPos, Timer / FadeTime);
	VCurtainTop->SetRelativePosition(TopPos);
	VCurtainBot->SetRelativePosition(BotPos);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
		SetActive(false);
	}
}

void AFadeCanvas::ProcessHCurtainOpen(float _DeltaTime)
{
	FVector LeftPos = UPokemonMath::Lerp(HCurtainLeftHidePos, HCurtainLeftShowPos, Timer / FadeTime);
	FVector RightPos = UPokemonMath::Lerp(HCurtainRightHidePos, HCurtainRightShowPos, Timer / FadeTime);
	HCurtainLeft->SetRelativePosition(LeftPos);
	HCurtainRight->SetRelativePosition(RightPos);

	if (Timer <= 0.0f)
	{
		State = EFadeCanvasState::Wait;
		SetActive(false);
	}
}
