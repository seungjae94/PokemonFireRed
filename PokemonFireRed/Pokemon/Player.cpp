#include "Player.h"
#include "PokemonInput.h"
#include "Map.h"
#include "Global.h"
#include "EventTarget.h"
#include "EventManager.h"

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AEventTarget::BeginPlay();

}

void APlayer::Tick(float _DeltaTime)
{
	AEventTarget::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void APlayer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EPlayerState::None:
		StateChange(EPlayerState::Idle);
		break;
	case EPlayerState::Idle:
		Idle(_DeltaTime);
		break;
	case EPlayerState::Walk:
		Walk(_DeltaTime);
		break;
	case EPlayerState::WalkInPlace:
		WalkInPlace(_DeltaTime);
		break;
	case EPlayerState::Jump:
		Jump(_DeltaTime);
		break;
	default:
		break;
	}
}

void APlayer::StateChange(EPlayerState _State, bool _Restart)
{
	if (false == _Restart && State == _State)
	{
		return;
	}

	switch (_State)
	{
	case EPlayerState::Idle:
		IdleStart();
		break;
	case EPlayerState::Walk:
		WalkStart();
		break;
	case EPlayerState::WalkInPlace:
		WalkInPlaceStart();
		break;
	case EPlayerState::Jump:
		JumpStart();
		break;
	default:
		break;
	}

	State = _State;
}

void APlayer::ChangeAnimation(EPlayerState _State, FTileVector _Direction)
{
	std::string AniName = GetName();

	switch (_State)
	{
	case EPlayerState::Idle:
		AniName += "Idle";
		break;
	case EPlayerState::Walk:
		AniName += "Walk";
		break;
	case EPlayerState::WalkInPlace:
		AniName += "WalkInPlace";
		break;
	case EPlayerState::Jump:
		AniName += "Jump";
		break;
	default:
		break;
	}

	std::string DirectionStr = _Direction.ToDirectionString();
	Renderer->ChangeAnimation(AniName + DirectionStr);
}

void APlayer::IdleStart(bool _ResetAnimation)
{
	if (_ResetAnimation == true)
	{
		ChangeAnimation(EPlayerState::Idle, Direction);
	}
	CurIdleTime = IdleTime;
}

void APlayer::Idle(float _DeltaTime)
{
	GetWorld()->SetCameraPos(GetActorLocation() - Global::HALF_SCREEN);
	if (CurIdleTime > 0.0f)
	{
		// Idle 상태 유지
		CurIdleTime -= _DeltaTime;
		return;
	}

	FTileVector InputDirection = PokemonInput::GetPressingDirection();

	// 1. 방향키를 누르지 않고 있다.
	if (InputDirection == FTileVector::Zero)
	{
		IdleStart(false);
		return;
	}

	// 2. 지금 보고 있는 방향과 다른 방향키를 누르고 있다.
	if (InputDirection != Direction)
	{
		Direction = InputDirection;
		IdleStart(true);
		return;
	}

	// 3. 앞에 Ledge가 있다.
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. 앞에 충돌체가 있다.
	if (IsCollider(Direction) == true)
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. 그 외의 경우
	StateChange(EPlayerState::Walk);
}

void APlayer::WalkStart(bool _ResetAnimation)
{
	if (_ResetAnimation == true)
	{
		ChangeAnimation(EPlayerState::Walk, Direction);
	}
	MemoryDirection = FTileVector::Zero;
	CurWalkTime = WalkTime;
	PrevPos = GetActorLocation();
	NextPos = PrevPos + Direction.ToFVector();
}

void APlayer::Walk(float _DeltaTime)
{
	// 1. 아직 이동 중이다.
	if (CurWalkTime > 0.0f)
	{
		IsExecutingMovingLogic = true;

		// 이동 로직
		CurWalkTime -= _DeltaTime;

		float t = (WalkTime - CurWalkTime) / WalkTime;

		FVector TargetPos = PokemonMath::Lerp(PrevPos, NextPos, t);
		FVector PlayerPos = GetActorLocation();
		FVector AddPos = TargetPos - PlayerPos;
		AddActorLocation(AddPos);
		GetWorld()->SetCameraPos(GetActorLocation() - Global::HALF_SCREEN);

		if (t >= WalkInputLatency)
		{
			MemoryDirection = PokemonInput::GetPressingDirection();
		}

		return;
	}
	IsExecutingMovingLogic = false;

	// 2. 기억하고 있는 입력 방향이 없다.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 3. 입력 방향에 Ledge가 있다.
	if (IsLedge(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. 입력 방향에 충돌체가 있다.
	if (IsCollider(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. 보고 있는 방향과 입력 방향이 다르다.
	if (MemoryDirection != Direction)
	{
		Direction = MemoryDirection;
		WalkStart(true);
		return;
	}

	// 6. 그 외의 경우
	WalkStart(false);
}

void APlayer::WalkInPlaceStart(bool _ResetAnimation)
{
	if (_ResetAnimation == true)
	{
		ChangeAnimation(EPlayerState::Walk, Direction);
	}
	CurWalkInPlaceTime = WalkInPlaceTime;
}

void APlayer::WalkInPlace(float _DeltaTime)
{
	if (CurWalkInPlaceTime > 0.0f)
	{
		CurWalkInPlaceTime -= _DeltaTime;
		return;
	}

	FTileVector InputDirection = PokemonInput::GetPressingDirection();

	// 1. 방향키를 누르고 있지 않다.
	if (InputDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 2. 지금 보고 있는 방향과 입력 방향이 같다.
	if (InputDirection == Direction)
	{
		WalkInPlaceStart(false);
		return;
	}

	// 3. 입력 방향에 Ledge가 있다.
	if (IsLedge(InputDirection))
	{
		Direction = InputDirection;
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. 입력 방향에 충돌체가 있다.
	if (IsCollider(InputDirection))
	{
		Direction = InputDirection;
		WalkInPlaceStart(true);
		return;
	}

	// 5. 그 외의 경우
	Direction = InputDirection;
	StateChange(EPlayerState::Walk);
}

void APlayer::JumpStart(bool _ResetAnimation)
{
	if (_ResetAnimation == true)
	{
		ChangeAnimation(EPlayerState::Jump, Direction);
	}
	MemoryDirection = FTileVector::Zero;
	CurJumpTime = JumpTime;
	PrevPos = GetActorLocation();
	NextPos = PrevPos + Direction.ToFVector() * 2;
}

void APlayer::Jump(float _DeltaTime)
{
	// 1. 아직 점프 중이다.
	if (CurJumpTime > 0.0f)
	{
		bool IsExecutingMovingLogic = true;

		// 이동 로직
		CurJumpTime -= _DeltaTime;

		float t = (JumpTime - CurJumpTime) / JumpTime;

		FVector TargetPos = PokemonMath::Lerp(PrevPos, NextPos, t);
		FVector PlayerPos = GetActorLocation();
		FVector AddPos = TargetPos - PlayerPos;
		AddActorLocation(AddPos);
		GetWorld()->SetCameraPos(GetActorLocation() - Global::HALF_SCREEN);

		if (t >= JumpInputLatency)
		{
			MemoryDirection = PokemonInput::GetPressingDirection();
		}

		return;
	}
	bool IsExecutingMovingLogic = false;

	// 2. 기억하고 있는 입력 방향이 없다.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 3. 입력 방향에 Ledge가 있고 바라보는 방향과 입력 방향이 다르다.
	if (IsLedge(MemoryDirection) == true && MemoryDirection != Direction)
	{
		Direction = MemoryDirection;
		JumpStart(true);
		return;
	}

	// 4. 입력 방향에 Ledge가 있고 바라보는 방향이 입력 방향과 같다.
	if (IsLedge(MemoryDirection) == true && MemoryDirection == Direction)
	{
		JumpStart(false);
		return;
	}

	// 5. 입력 방향에 충돌체가 있다.
	if (IsCollider(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 6. 그 외의 경우
	Direction = MemoryDirection;
	StateChange(EPlayerState::Walk);
}

bool APlayer::IsLedge(FTileVector _Direction)
{
	// 맵(이미지 좌상단)을 기준으로 한 타겟의 상대 좌표
	FVector MapRelativeTargetPos = (GetActorLocation() - Map->GetActorLocation()) + _Direction.ToFVector();
	FVector MapRelativeTargetPosInImage = MapRelativeTargetPos * (1 / Global::F_MAP_RUNTIME_SCALE_FACTOR);
	Color8Bit Color = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPosInImage.iX(), 
		MapRelativeTargetPosInImage.iY(), 
		Color8Bit::WhiteA
	);

	if (_Direction == FTileVector::Down)
	{
		return Color == Color8Bit(255, 255, 0, 0);
	}

	return false;
}

bool APlayer::IsCollider(FTileVector _Direction)
{
	FVector MapRelativeTargetPos = (GetActorLocation() - Map->GetActorLocation()) + _Direction.ToFVector();
	FVector MapRelativeTargetPosInImage = MapRelativeTargetPos * (1 / Global::F_MAP_RUNTIME_SCALE_FACTOR);
	Color8Bit Color = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPosInImage.iX(),
		MapRelativeTargetPosInImage.iY(),
		Color8Bit::MagentaA
	);

	return Color == Color8Bit(255, 0, 255, 0) || (Color.R == 255 && Color.G == 255);
}