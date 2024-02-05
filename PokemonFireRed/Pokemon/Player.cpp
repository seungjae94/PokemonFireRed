#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include "Map.h"
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

	// 플레이어 이미지 세팅
	Renderer = CreateImageRenderer(0);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransColor(Color8Bit::White.ZeroAlphaColor());
	Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
	Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });

	// 애니메이션 생성
	Renderer->CreateAnimation("IdleLeft", "IdleLeft.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleRight", "IdleRight.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleUp", "IdleUp.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleDown", "IdleDown.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("WalkLeft", "WalkLeft.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkRight", "WalkRight.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkUp", "WalkUp.png", 0, 2, WalkInterval, true);
	Renderer->CreateAnimation("WalkDown", "WalkDown.png", 0, 2, WalkInterval, true);
}

void APlayer::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void APlayer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EPlayerState::Idle:
		Idle(_DeltaTime);
		break;
	case EPlayerState::Walk:
		Walk(_DeltaTime);
		break;
	case EPlayerState::Jump:
		break;
	default:
		break;
	}
}

void APlayer::Idle(float _DeltaTime)
{
	FIntPoint NextDirection = FIntPoint::Zero;
	if (EngineInput::IsPress(VK_DOWN))
	{
		NextDirection = FIntPoint::Down;
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		NextDirection = FIntPoint::Up;
	}
	else if (EngineInput::IsPress(VK_LEFT))
	{
		NextDirection = FIntPoint::Left;
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		NextDirection = FIntPoint::Right;
	}
	else
	{
		std::string DirectionStr = Direction.ToString();
		Renderer->ChangeAnimation("Idle" + DirectionStr);
		return;
	}

	if (Direction == NextDirection)
	{
		// 바라보는 방향의 키를 누른 경우
		// - 한 칸 이동한다.
		// - 다음 방향의 걷기 애니메이션을 재생한다.
		std::string DirectionStr = Direction.ToString();
		Renderer->ChangeAnimation("Walk" + DirectionStr);
		WorldPos += Direction;
		State = EPlayerState::Walk;
	}
	else
	{
		// 바라보지 않는 방향의 키를 누른 경우
		// - 방향을 바꾼다.
		// - 다음 방향의 정지 애니메이션을 재생한다.
		Direction = NextDirection;
		std::string DirectionStr = Direction.ToString();
		Renderer->ChangeAnimation("Idle" + DirectionStr);
	}
}

void APlayer::Walk(float _DeltaTime)
{
	static float CurWalkTime = WalkTime;

	FIntPoint NextDirection = FIntPoint::Zero;
	if (EngineInput::IsPress(VK_DOWN))
	{
		NextDirection = FIntPoint::Down;
	}
	else if (EngineInput::IsPress(VK_UP))
	{
		NextDirection = FIntPoint::Up;
	}
	else if (EngineInput::IsPress(VK_LEFT))
	{
		NextDirection = FIntPoint::Left;
	}
	else if (EngineInput::IsPress(VK_RIGHT))
	{
		NextDirection = FIntPoint::Right;
	}

	if (true == IsMoving)
	{
		CurWalkTime -= _DeltaTime;

		FVector MoveVector = Direction.ToFVector() * speed * Global::TILE_SIZE * _DeltaTime;
		Map->AddActorLocation(MoveVector * (-1));

		if (CurWalkTime <= 0.0f)
		{
			CurWalkTime = WalkTime;
			IsMoving = false;

			if (NextDirection == FIntPoint::Zero)
			{
				State = EPlayerState::Idle;
			}
		}

		return;
	}

	if (Direction == NextDirection)
	{
		// 바라보는 방향의 키를 누른 경우
		// - 한 칸 이동한다.
	}
	else
	{
		// 바라보지 않는 방향의 키를 누른 경우
		// - 방향을 바꾼다.
		// - 새로운 걷기 애니메이션을 재생한다.
		// - 앞으로 한 칸 이동한다.
		Direction = NextDirection;
		std::string DirectionStr = Direction.ToString();
		Renderer->ChangeAnimation("Walk" + DirectionStr);
	}

	WorldPos += Direction;

	IsMoving = true;
}




