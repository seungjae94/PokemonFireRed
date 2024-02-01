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
	Renderer->CreateAnimation("IdleLeft", "IdleLeft.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleRight", "IdleRight.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleUp", "IdleUp.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleDown", "IdleDown.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("WalkLeft", "WalkLeft.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkRight", "WalkRight.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkUp", "WalkUp.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkDown", "WalkDown.png", 0, 2, WalkInterval, true);
}

bool Equal(const FVector& _Left, const FVector& _Right)
{
	return _Left.X == _Right.X && _Left.Y == _Right.Y;
}

void APlayer::Tick(float _DeltaTime)
{
	static bool IsWalking = false;

	if (IsWalking)
	{
		CurWalkTime -= _DeltaTime;

		if (CurWalkTime <= 0.0f)
		{
			IsWalking = false;
			CurWalkTime = WalkTime;
		}

		return;
	}

	if (EngineInput::IsPress(VK_LEFT))
	{
		if (true == IsIdle || false == Equal(Direction, FVector::Left))
		{
			Renderer->ChangeAnimation("WalkLeft");
		}
		
		Direction = FVector::Left;
		WorldPos += FVector::Left;
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		if (true == IsIdle || false == Equal(Direction, FVector::Right))
		{
			Renderer->ChangeAnimation("WalkRight");
		}

		Direction = FVector::Right;
		WorldPos += FVector::Right;
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		if (true == IsIdle || false == Equal(Direction, FVector::Up))
		{
			Renderer->ChangeAnimation("WalkUp");
		}

		Direction = FVector::Up;
		WorldPos += FVector::Up;
	}
	else if (EngineInput::IsPress(VK_DOWN))
	{
		if (true == IsIdle || false == Equal(Direction, FVector::Down))
		{
			Renderer->ChangeAnimation("WalkDown");
		}

		Direction = FVector::Down;
		WorldPos += FVector::Down;
	}
	else
	{
		if (true == Equal(Direction, FVector::Left))
		{
			Renderer->ChangeAnimation("IdleLeft");
		}
		else if (true == Equal(Direction, FVector::Right))
		{
			Renderer->ChangeAnimation("IdleRight");
		}
		else if (true == Equal(Direction, FVector::Up))
		{
			Renderer->ChangeAnimation("IdleUp");
		}
		else if (true == Equal(Direction, FVector::Down))
		{
			Renderer->ChangeAnimation("IdleDown");
		}
		IsIdle = true;
		return;
	}

	IsIdle = false;
	IsWalking = true;
}


