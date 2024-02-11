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
		// Idle ���� ����
		CurIdleTime -= _DeltaTime;
		return;
	}

	FTileVector InputDirection = PokemonInput::GetPressingDirection();

	// 1. ����Ű�� ������ �ʰ� �ִ�.
	if (InputDirection == FTileVector::Zero)
	{
		IdleStart(false);
		return;
	}

	// 2. ���� ���� �ִ� ����� �ٸ� ����Ű�� ������ �ִ�.
	if (InputDirection != Direction)
	{
		Direction = InputDirection;
		IdleStart(true);
		return;
	}

	// 3. �տ� Ledge�� �ִ�.
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. �տ� �浹ü�� �ִ�.
	if (IsCollider(Direction) == true)
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. �� ���� ���
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
	// 1. ���� �̵� ���̴�.
	if (CurWalkTime > 0.0f)
	{
		IsExecutingMovingLogic = true;

		// �̵� ����
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

	// 2. ����ϰ� �ִ� �Է� ������ ����.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 3. �Է� ���⿡ Ledge�� �ִ�.
	if (IsLedge(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. �Է� ���⿡ �浹ü�� �ִ�.
	if (IsCollider(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. ���� �ִ� ����� �Է� ������ �ٸ���.
	if (MemoryDirection != Direction)
	{
		Direction = MemoryDirection;
		WalkStart(true);
		return;
	}

	// 6. �� ���� ���
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

	// 1. ����Ű�� ������ ���� �ʴ�.
	if (InputDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 2. ���� ���� �ִ� ����� �Է� ������ ����.
	if (InputDirection == Direction)
	{
		WalkInPlaceStart(false);
		return;
	}

	// 3. �Է� ���⿡ Ledge�� �ִ�.
	if (IsLedge(InputDirection))
	{
		Direction = InputDirection;
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. �Է� ���⿡ �浹ü�� �ִ�.
	if (IsCollider(InputDirection))
	{
		Direction = InputDirection;
		WalkInPlaceStart(true);
		return;
	}

	// 5. �� ���� ���
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
	// 1. ���� ���� ���̴�.
	if (CurJumpTime > 0.0f)
	{
		bool IsExecutingMovingLogic = true;

		// �̵� ����
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

	// 2. ����ϰ� �ִ� �Է� ������ ����.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 3. �Է� ���⿡ Ledge�� �ְ� �ٶ󺸴� ����� �Է� ������ �ٸ���.
	if (IsLedge(MemoryDirection) == true && MemoryDirection != Direction)
	{
		Direction = MemoryDirection;
		JumpStart(true);
		return;
	}

	// 4. �Է� ���⿡ Ledge�� �ְ� �ٶ󺸴� ������ �Է� ����� ����.
	if (IsLedge(MemoryDirection) == true && MemoryDirection == Direction)
	{
		JumpStart(false);
		return;
	}

	// 5. �Է� ���⿡ �浹ü�� �ִ�.
	if (IsCollider(MemoryDirection) == true)
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 6. �� ���� ���
	Direction = MemoryDirection;
	StateChange(EPlayerState::Walk);
}

bool APlayer::IsLedge(FTileVector _Direction)
{
	// ��(�̹��� �»��)�� �������� �� Ÿ���� ��� ��ǥ
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