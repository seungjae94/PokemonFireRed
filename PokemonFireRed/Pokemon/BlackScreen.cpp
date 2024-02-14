#include "BlackScreen.h"
#include "Global.h"
#include <EngineCore/EngineResourcesManager.h>

ABlackScreen::ABlackScreen() 
{
}

ABlackScreen::~ABlackScreen() 
{
}

void ABlackScreen::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	Renderer->CameraEffectOff();
	Renderer->SetImage("BlackScreen.png");
	Renderer->SetTransform({ Global::HALF_SCREEN, Global::SCREEN});
	Renderer->SetTransColor(Color8Bit::WhiteA);
	Renderer->SetAlpha(0.0f);
	Renderer->SetActive(false);
}

void ABlackScreen::Tick(float _DeltaTime)
{
}

