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

	SetActorLocation({ SCREEN_X/2, SCREEN_Y/2});

	Renderer = CreateImageRenderer(1);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransform({ {0, -TILE_SIZE / 2}, {TILE_SIZE, 2 * TILE_SIZE} });
	Renderer->SetImageCuttingTransform({{0, 0}, {IMAGE_TILE_SIZE, 2*IMAGE_TILE_SIZE}});
}

void APlayer::Tick(float _DeltaTime)
{
	static float speed = 3 * TILE_SIZE;

	FVector MoveVec = FVector::Zero;
	if (EngineInput::IsPress(VK_LEFT))
	{
		MoveVec = FVector::Left * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		MoveVec = FVector::Right * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		MoveVec = FVector::Up * speed * _DeltaTime;
	}
	else if (EngineInput::IsPress(VK_DOWN))
	{
		MoveVec = FVector::Down * speed * _DeltaTime;
	}

	//AddActorLocation(MoveVec);
	Ground->AddActorLocation(MoveVec * (-1));
}


