#include "Ground.h"

#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "Player.h"

AGround::AGround()
{
}

AGround::~AGround()
{
}

void AGround::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(0);
	Renderer->SetImage("PalletTown.png");

	UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg("PalletTown.png");
	FVector Scale =  Image->GetScale();
	RenderScale = Scale * Global::F_PIXEL_SIZE;

	TileCount.X = RenderScale.X / Global::TILE_SIZE;
	TileCount.Y = RenderScale.Y / Global::TILE_SIZE;

	Renderer->SetTransform({ {0, 0}, RenderScale });
	Renderer->SetImageCuttingTransform({ {0, 0}, Scale });
}

void AGround::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	
	UpdateScreenPos();
}

void AGround::UpdateScreenPos()
{
	FVector ScreenPos = { Global::HALF_SCREEN_X, Global::HALF_SCREEN_Y };
	ScreenPos += RenderScale * 0.5f;										// ��� �»�ܰ� ȭ�� �߾��� ��ġ��Ų��.
	ScreenPos += LTWorldPos * Global::F_TILE_SIZE;							// ��� �»���� ���� ��ǥ�� ���Ѵ�.
	ScreenPos -= Player->GetWorldPos() * Global::F_TILE_SIZE;				// �÷��̾��� ���� ��ǥ�� ����.
	SetActorLocation(ScreenPos);
}
