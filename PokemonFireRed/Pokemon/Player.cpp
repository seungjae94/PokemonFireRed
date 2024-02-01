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

	// 걷기 애니메이션 생성
	int WalkInterval = 1.0f;
	Renderer->CreateAnimation("WalkLeft", "WalkLeft.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkRight", "WalkRight.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkUp", "WalkUp.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkDown", "WalkDown.png", 0, 2, WalkInterval, true);
}

void APlayer::Tick(float _DeltaTime)
{
	if (EngineInput::IsDown(VK_LEFT))
	{
		WorldPos += FVector::Left;
		Renderer->ChangeAnimation("WalkLeft");
	}
	else if (EngineInput::IsDown(VK_RIGHT))
	{
		WorldPos += FVector::Right;
		Renderer->ChangeAnimation("WalkRight");
	}
	else if (EngineInput::IsDown(VK_UP))
	{
		WorldPos += FVector::Up;
		Renderer->ChangeAnimation("WalkUp");
	}
	else if (EngineInput::IsDown(VK_DOWN))
	{
		WorldPos += FVector::Down;
		Renderer->ChangeAnimation("WalkDown");
	}
}


