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

	BackgroundRenderer->SetTransform({ Scale.Half2D(), Scale});
	BackgroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetForegroundImage(std::string_view _Name)
{
	ForegroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();

	ForegroundRenderer->SetTransform({ Scale.Half2D(), Scale });
	ForegroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetCollisionImage(std::string_view _Name)
{
	CollisionRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();

	CollisionRenderer->SetTransform({ Scale.Half2D(), Scale });
	CollisionRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::BeginPlay()
{
	AActor::BeginPlay();

	BackgroundRenderer = CreateImageRenderer(ERenderingOrder::Background);
	ForegroundRenderer = CreateImageRenderer(ERenderingOrder::Foreground);
	CollisionRenderer = CreateImageRenderer(ERenderingOrder::None);
}

void AMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}