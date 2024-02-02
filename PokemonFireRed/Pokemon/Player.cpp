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

	WorldPos = { 0, 0 };
	SetActorLocation({ Global::SCREEN_X / 2, Global::SCREEN_Y / 2 });

	Renderer = CreateImageRenderer(0);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransColor(Color8Bit::White.ZeroAlphaColor());
	Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
	Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });

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
	static bool IsDelayInput = false;
	static bool IsWalking = false;
	static float CurWalkTime = WalkTime;
	static float CurDelayInputTime = DelayInputTime;

	if (IsDelayInput)
	{
		CurDelayInputTime -= _DeltaTime;

		if (CurDelayInputTime <= 0.0f)
		{
			IsDelayInput = false;
			CurDelayInputTime = DelayInputTime;
		}

		return;
	}

	if (IsWalking)
	{
		CurWalkTime -= _DeltaTime;
		Ground->AddActorLocation(Direction * -speed * Global::TILE_SIZE * _DeltaTime);

		if (CurWalkTime <= 0.0f)
		{
			IsWalking = false;
			CurWalkTime = WalkTime;
		}

		return;
	}

	//방향이 같은 경우:
	//	정지 상태였던 경우:
	//		걷기 애니메이션 재생 + 이동
	//	정지 상태가 아니었던 경우:
	//		이동

	//방향이 다른 경우:
	//	정지 상태였던 경우:
	//		방향 변경 + 정지 애니메이션 재생
	//	정지 상태가 아니었던 경우:
	//		방향 변경 + 걷기 애니메이션 재생 + 이동
	if (EngineInput::IsPress(VK_LEFT))
	{
		if (Equal(Direction, FVector::Left))
		{
			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("WalkLeft");
			}

			WorldPos += FVector::Left;
		}
		else
		{
			Direction = FVector::Left;

			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("IdleLeft");
				IsDelayInput = true;
				return;
			}
			else
			{
				Renderer->ChangeAnimation("WalkLeft");
				WorldPos += FVector::Left;
			}
		}
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		if (Equal(Direction, FVector::Right))
		{
			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("WalkRight");
			}

			WorldPos += FVector::Right;
		}
		else
		{
			Direction = FVector::Right;

			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("IdleRight");
				IsDelayInput = true;
				return;
			}
			else
			{
				Renderer->ChangeAnimation("WalkRight");
				WorldPos += FVector::Right;
			}
		}
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		if (Equal(Direction, FVector::Up))
		{
			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("WalkUp");
			}

			WorldPos += FVector::Up;
		}
		else
		{
			Direction = FVector::Up;

			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("IdleUp");
				IsDelayInput = true;
				return;
			}
			else
			{
				Renderer->ChangeAnimation("WalkUp");
				WorldPos += FVector::Up;
			}
		}
	}
	else if (EngineInput::IsPress(VK_DOWN))
	{
		if (Equal(Direction, FVector::Down))
		{
			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("WalkDown");
			}

			WorldPos += FVector::Down;
		}
		else
		{
			Direction = FVector::Down;

			if (true == IsIdle)
			{
				Renderer->ChangeAnimation("IdleDown");
				IsDelayInput = true;
				return;
			}
			else
			{
				Renderer->ChangeAnimation("WalkDown");
				WorldPos += FVector::Down;
			}
		}
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


