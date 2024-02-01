#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include "Ground.h"
#include "Global.h"

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AActor::BeginPlay();

	WorldPos = {0, 0};
	SetActorLocation({ Global::SCREEN_X / 2, Global::SCREEN_Y/2});

	Renderer = CreateImageRenderer(1);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
	Renderer->SetImageCuttingTransform({{0, 0}, {Global::IMAGE_TILE_SIZE, 2* Global::IMAGE_TILE_SIZE}});
}

void APlayer::Tick(float _DeltaTime)
{
	if (EngineInput::IsDown(VK_LEFT))
	{
		WorldPos += FVector::Left;
	}
	else if (EngineInput::IsDown(VK_RIGHT))
	{
		WorldPos += FVector::Right;
	}
	else if (EngineInput::IsDown(VK_UP))
	{
		WorldPos += FVector::Up;
	}
	else if (EngineInput::IsDown(VK_DOWN))
	{
		WorldPos += FVector::Down;
	}
}


