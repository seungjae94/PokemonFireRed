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
	Renderer->SetTransform({ {0, 0}, Global::SCREEN});
}

void ABlackScreen::Tick(float _DeltaTime)
{
}

