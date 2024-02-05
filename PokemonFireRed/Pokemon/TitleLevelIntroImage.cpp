#include "TitleLevelIntroImage.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineCore/ImageRenderer.h>
#include "Global.h"

UTitleLevelIntroImage::UTitleLevelIntroImage() 
{
}

UTitleLevelIntroImage::~UTitleLevelIntroImage() 
{
}

void UTitleLevelIntroImage::BeginPlay()
{
	Renderer = CreateImageRenderer();

	FVector ScreenCenter = GEngine->MainWindow.GetWindowScale() * 0.5f;

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg("IntroTempImage.png");
	FVector ImageScale = Image->GetScale();
	FVector ImageRenderScale = ImageScale * Global::PIXEL_SIZE;

	Renderer->SetImage("IntroTempImage.png");
	Renderer->SetTransColor(Color8Bit::White);
	Renderer->SetTransform({ ImageRenderScale.Half2D(), {ImageRenderScale}});
	Renderer->SetImageCuttingTransform({ {0, 0}, ImageScale });
}

void UTitleLevelIntroImage::Tick(float _DeltaTime)
{
	if (EngineInput::IsDown('Z'))
	{
		GEngine->ChangeLevel("ExteriorPalletTownLevel");
	}
}
