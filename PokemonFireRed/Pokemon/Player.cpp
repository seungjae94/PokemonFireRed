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

	SetActorLocation({ 0, 0 });

	FVector CameraInitialPos = { -Global::F_HALF_SCREEN_X, -Global::F_HALF_SCREEN_Y };
	GetWorld()->SetCameraPos(CameraInitialPos);

	// 플레이어 이미지 세팅
	Renderer = CreateImageRenderer(ERenderingOrder::Lower);
	Renderer->SetImage("Player.bmp");
	Renderer->SetTransColor(Color8Bit::White.ZeroAlphaColor());
	Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
	Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });

	// 애니메이션 생성
	Renderer->CreateAnimation("IdleLeft", "IdleLeft.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleRight", "IdleRight.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleUp", "IdleUp.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("IdleDown", "IdleDown.png", 0, 0, 0.0f, false);
	Renderer->CreateAnimation("WalkLeft", "WalkLeft.png", 0, 3, WalkInterval, true);
	Renderer->CreateAnimation("WalkRight", "WalkRight.png", 0, 3, WalkInterval, true);
	Renderer->CreateAnimation("WalkUp", "WalkUp.png", 0, 3, WalkInterval, true);
	Renderer->CreateAnimation("WalkDown", "WalkDown.png", 0, 3, WalkInterval, true);
	Renderer->CreateAnimation("JumpDown", "JumpDown.png", 0, 52, WalkInterval / 8.0f, true);
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
		Jump(_DeltaTime);
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

	if (NextDirection == FIntPoint::Zero)
	{
		// 키를 누르지 않은 경우
		return;
	}

	if (Direction == NextDirection)
	{
		// 바라보는 방향의 키를 누른 경우
		// - 한 칸 이동한다.
		// - 다음 방향의 걷기 애니메이션을 재생한다.
		std::string DirectionStr = Direction.ToString();
		Renderer->ChangeAnimation("Walk" + DirectionStr);
		State = EPlayerState::Walk;

		// 충돌이 있을 경우 이동하지 않는다.
		if (true == CheckCollision())
		{
			return;
		}

		IsMoving = true;
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

	if (true == CheckJump())
	{
		State = EPlayerState::Jump;
		return;
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
		AddActorLocation(MoveVector);
		GetWorld()->AddCameraPos(MoveVector);

		if (CurWalkTime <= 0.0f)
		{
			CurWalkTime = WalkTime;
			IsMoving = false;
		}

		return;
	}

	if (NextDirection == FIntPoint::Zero)
	{
		// 키를 누르지 않은 경우
		State = EPlayerState::Idle;
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


	if (true == CheckJump())
	{
		State = EPlayerState::Jump;
		return;
	}

	// 충돌이 있을 경우 이동하지 않는다.
	if (true == CheckCollision())
	{
		return;
	}

	IsMoving = true;
}

void APlayer::Jump(float _DeltaTime)
{
	static float CurJumpTime = JumpTime;

	if (false == IsMoving)
	{
		Renderer->ChangeAnimation("JumpDown");
		IsMoving = true;
	}
	else
	{
		CurJumpTime -= _DeltaTime;

		FVector MoveVector = Direction.ToFVector() * speed * Global::TILE_SIZE * _DeltaTime;
		AddActorLocation(MoveVector);
		GetWorld()->AddCameraPos(MoveVector);

		if (CurJumpTime <= 0.0f)
		{
			CurJumpTime = JumpTime;
			IsMoving = false;
			State = EPlayerState::Walk;
		}

		return;
	}
	
}

bool APlayer::CheckCollision()
{
	FVector TargetPos = GetActorLocation() + Direction.ToFVector() * Global::F_TILE_SIZE;
	FVector MapRelativePos = TargetPos - Map->GetActorLocation();
	Color8Bit Color = Map->GetCollisionImage()->GetColor(MapRelativePos.iX(), MapRelativePos.iY(), Color8Bit::MagentaA);

	return Color == Color8Bit(255, 0, 255, 0)
		|| (Color.R == 255 && Color.G == 255 );
}

bool APlayer::CheckJump()
{
	FVector TargetPos = GetActorLocation() + Direction.ToFVector() * Global::F_TILE_SIZE;
	FVector MapRelativePos = TargetPos - Map->GetActorLocation();
	Color8Bit Color = Map->GetCollisionImage()->GetColor(MapRelativePos.iX(), MapRelativePos.iY(), Color8Bit::WhiteA);

	if (Direction == FIntPoint::Down)
	{
		return Color == Color8Bit(255, 255, 0, 0);
	}

	return false;
}




