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
	RenderScale = Scale;

	TileCount.X = RenderScale.X / Global::TILE_SIZE;
	TileCount.Y = RenderScale.Y / Global::TILE_SIZE;

	BackgroundRenderer->SetTransform({ {0, 0}, RenderScale });
	BackgroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetForegroundImage(std::string_view _Name)
{
	ForegroundRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	RenderScale = Scale;

	ForegroundRenderer->SetTransform({ {0, 0}, RenderScale });
	ForegroundRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::SetCollisionImage(std::string_view _Name)
{
	CollisionRenderer->SetImage(_Name);

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(_Name);
	FVector Scale = Image->GetScale();
	RenderScale = Scale;

	CollisionRenderer->SetTransform({ {0, 0}, RenderScale });
	CollisionRenderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AMap::BeginPlay()
{
	AActor::BeginPlay();

	BackgroundRenderer = CreateImageRenderer(-1000);
	ForegroundRenderer = CreateImageRenderer(1000);
	CollisionRenderer = CreateImageRenderer(0);
}

void AMap::Tick(float _DeltaTime)
{
	static bool FirstUpdate = false;

	AActor::Tick(_DeltaTime);

	if (false == FirstUpdate)
	{
		FirstUpdate = true;
		SyncGroundScreenPosToPlayerWorldPos();
	}

}

void AMap::SyncGroundScreenPosToPlayerWorldPos()
{
	FVector ScreenPos = { Global::HALF_SCREEN_X, Global::HALF_SCREEN_Y };
	ScreenPos += RenderScale * 0.5f;										// 배경 좌상단과 화면 중앙을 일치시킨다.
	ScreenPos += WorldPos * Global::F_TILE_SIZE;							// 배경 좌상단의 월드 좌표를 더한다.
	ScreenPos -= Player->GetWorldPos().ToFVector() * Global::F_TILE_SIZE;				// 플레이어의 월드 좌표를 뺀다.
	SetActorLocation(ScreenPos);
}
