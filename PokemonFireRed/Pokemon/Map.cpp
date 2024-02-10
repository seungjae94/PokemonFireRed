#include "Map.h"

#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "Player.h"

AMap::AMap()
{
}

AMap::~AMap()
{
}

void AMap::SetBackgroundImage(std::string_view _Name)
{
	BackgroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	FVector RenderScale = Scale * Global::F_MAP_RUNTIME_SCALE_FACTOR;

	BackgroundRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
	BackgroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetForegroundImage(std::string_view _Name)
{
	ForegroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	FVector RenderScale = Scale * Global::F_MAP_RUNTIME_SCALE_FACTOR;

	ForegroundRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
	ForegroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetCollisionImage(std::string_view _Name)
{
	CollisionRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	FVector RenderScale = Scale * Global::F_MAP_RUNTIME_SCALE_FACTOR;

	CollisionRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
	CollisionRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::BeginPlay()
{
	AActor::BeginPlay();

	// ¸Ê ·»´õ¸µ
	BackgroundRenderer = CreateImageRenderer(ERenderingOrder::Background);
	ForegroundRenderer = CreateImageRenderer(ERenderingOrder::Foreground);
	CollisionRenderer = CreateImageRenderer(ERenderingOrder::Super);
}

void AMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}