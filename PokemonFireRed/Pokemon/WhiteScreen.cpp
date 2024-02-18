#include "WhiteScreen.h"
#include "Global.h"

AWhiteScreen::AWhiteScreen()
{
}

AWhiteScreen::~AWhiteScreen()
{
}

void AWhiteScreen::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(ERenderingOrder::Fade);
	Renderer->CameraEffectOff();
	Renderer->SetImage("WhiteScreen.png");
	Renderer->SetTransform({ Global::HalfScreen, Global::Screen });
	Renderer->SetAlpha(0.0f);
	Renderer->SetActive(false);
}

void AWhiteScreen::Tick(float _DeltaTime)
{
	switch (State)
	{
	case AFadeScreen::EFadeState::Hide:
		Renderer->SetAlpha(0.0f);
		AllRenderersActiveOff();
		break;
	case AFadeScreen::EFadeState::FadeIn:
		FadeInTick(_DeltaTime);
		break;
	case AFadeScreen::EFadeState::FadeOut:
		FadeOutTick(_DeltaTime);
		break;
	case AFadeScreen::EFadeState::Show:
		Renderer->SetAlpha(1.0f);
		AllRenderersActiveOn();
		break;
	default:
		break;
	}
}

void AWhiteScreen::FadeInTick(float _DeltaTime)
{
	if (CurFadeTime <= 0.0f)
	{
		State = EFadeState::Hide;
		return;
	}

	CurFadeTime -= _DeltaTime;
	Renderer->SetAlpha(CurFadeTime / FadeTime);
}

void AWhiteScreen::FadeOutTick(float _DeltaTime)
{
	if (CurFadeTime <= 0.0f)
	{
		State = EFadeState::Show;
		return;
	}

	CurFadeTime -= _DeltaTime;
	Renderer->SetAlpha((FadeTime - CurFadeTime) / FadeTime);
}


