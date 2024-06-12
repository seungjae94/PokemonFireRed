#include "Map.h"

#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "PlayerCharacter.h"

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
	FVector RenderScale = Scale;

	BackgroundRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
}

void AMap::SetForegroundImage(std::string_view _Name)
{
	ForegroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	FVector RenderScale = Scale;

	ForegroundRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
}

void AMap::SetCollisionImage(std::string_view _Name)
{
	CollisionRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	FVector RenderScale = Scale;

	CollisionRenderer->SetTransform({ RenderScale.Half2D(), RenderScale });
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