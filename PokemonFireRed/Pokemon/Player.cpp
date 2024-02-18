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
	std::string UpperAniName = GetName();
	std::string LowerAniName = GetName();
	std::string DirectionStr = _Direction.ToDirectionString();

	switch (_State)
	{
	case EPlayerState::Idle:
		UpperAniName += "Idle" + DirectionStr + Global::SuffixUpperBody;
		LowerAniName += "Idle" + DirectionStr + Global::SuffixLowerBody;
		break;
	case EPlayerState::Walk:
		UpperAniName += "Walk" + DirectionStr + Global::SuffixUpperBody + std::to_string(FootOrder);
		LowerAniName += "Walk" + DirectionStr + Global::SuffixLowerBody + std::to_string(FootOrder);
		IncFootOrder();
		break;
	case EPlayerState::WalkInPlace:
		UpperAniName += "Walk" + DirectionStr + Global::SuffixUpperBody + std::to_string(FootOrder);
		LowerAniName += "Walk" + DirectionStr + Global::SuffixLowerBody + std::to_string(FootOrder);
		IncFootOrder();
		break;
	case EPlayerState::Jump:
		UpperAniName += "Jump" + DirectionStr + Global::SuffixUpperBody;
		LowerAniName += "Jump" + DirectionStr + Global::SuffixLowerBody;
		break;
	default:
		break;
	}

	UpperBodyRenderer->ChangeAnimation(UpperAniName);
	LowerBodyRenderer->ChangeAnimation(LowerAniName);
}

void APlayer::IdleStart()
{
	ChangeAnimation(EPlayerState::Idle, Direction);
	CurIdleTime = IdleTime;
}

void APlayer::Idle(float _DeltaTime)
{
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
		IdleStart();
		return;
	}

	// 2. ���� ���� �ִ� ����� �ٸ� ����Ű�� ������ �ִ�.
	if (InputDirection != Direction)
	{
		Direction = InputDirection;
		IdleStart();
		return;
	}

	// 3. �տ� Ledge�� �ִ�.
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 4. �տ� �浹ü(�ȼ� ��� or ������Ʈ ���)�� �ִ�.
	if (true == IsCollider(Direction))
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. �� ���� ���
	StateChange(EPlayerState::Walk);
}

void APlayer::WalkStart()
{
	ChangeAnimation(EPlayerState::Walk, Direction);
	MemoryDirection = FTileVector::Zero;
	CurWalkTime = WalkTime;
	PrevPoint = FTileVector(GetActorLocation());
	NextPoint = PrevPoint + Direction;
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

		FVector TargetPos = UPokemonMath::Lerp(PrevPoint, NextPoint, t);
		SetActorLocation(TargetPos);
		GetWorld()->SetCameraPos(GetActorLocation() - Global::HalfScreen);

		if (t >= WalkInputLatency)
		{
			MemoryDirection = PokemonInput::GetPressingDirection();
		}

		return;
	}
	IsExecutingMovingLogic = false;
	UEventManager::SetPoint(GetWorld()->GetName(), GetName(), NextPoint);

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
	if (true == IsCollider(MemoryDirection))
	{
		Direction = MemoryDirection;
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 5. ���� �ִ� ����� �Է� ������ �ٸ���.
	if (MemoryDirection != Direction)
	{
		Direction = MemoryDirection;
		WalkStart();
		return;
	}

	// 6. �� ���� ���
	WalkStart();
}

void APlayer::WalkInPlaceStart()
{
	ChangeAnimation(EPlayerState::Walk, Direction);
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
		WalkInPlaceStart();
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
	if (true == IsCollider(InputDirection))
	{
		Direction = InputDirection;
		WalkInPlaceStart();
		return;
	}

	// 5. �� ���� ���
	Direction = InputDirection;
	StateChange(EPlayerState::Walk);
}

void APlayer::JumpStart()
{
	ChangeAnimation(EPlayerState::Jump, Direction);
	MemoryDirection = FTileVector::Zero;
	CurJumpTime = JumpTime;
	PrevPoint = FTileVector(GetActorLocation());
	NextPoint = PrevPoint + Direction.ToFVector() * 2;
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

		FVector TargetPos = UPokemonMath::Lerp(PrevPoint, NextPoint, t);
		SetActorLocation(TargetPos);
		GetWorld()->SetCameraPos(GetActorLocation() - Global::HalfScreen);

		if (t >= JumpInputLatency)
		{
			MemoryDirection = PokemonInput::GetPressingDirection();
		}

		return;
	}
	bool IsExecutingMovingLogic = false;
	UEventManager::SetPoint(GetWorld()->GetName(), GetName(), NextPoint);

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
		JumpStart();
		return;
	}

	// 4. �Է� ���⿡ Ledge�� �ְ� �ٶ󺸴� ������ �Է� ����� ����.
	if (IsLedge(MemoryDirection) == true && MemoryDirection == Direction)
	{
		JumpStart();
		return;
	}

	// 5. �Է� ���⿡ �浹ü�� �ִ�.
	if (true == IsCollider(MemoryDirection))
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
	FVector MapRelativeTargetPosInImage = MapRelativeTargetPos;
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

bool APlayer::IsPixelCollider(FTileVector _Direction)
{
	FVector MapRelativeTargetPos = (GetActorLocation() - Map->GetActorLocation()) + _Direction.ToFVector();
	FVector MapRelativeTargetPosInImage = MapRelativeTargetPos;
	Color8Bit Color = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPosInImage.iX(),
		MapRelativeTargetPosInImage.iY(),
		Color8Bit::MagentaA
	);

	return Color == Color8Bit(255, 0, 255, 0) || (Color.R == 255 && Color.G == 255);
}

bool APlayer::IsComponentCollider(FTileVector _Direction)
{
	std::vector<UCollision*> CollisionResult;
	
	bool IsCollided = Collision->CollisionCheck(ECollisionOrder::NPC, CollisionResult);
	
	FTileVector TestPoint = GetPoint() + _Direction;
	for (UCollision* Collision : CollisionResult)
	{
		AEventTarget* Target = dynamic_cast<AEventTarget*>(Collision->GetOwner());

		if (nullptr == Target)
		{
			MsgBoxAssert("�ݸ����� ���� ������ �̺�Ʈ Ÿ���� �ƴ� ������Ʈ" + Collision->GetOwner()->GetName() + "�� �����մϴ�.");
			return false;
		}

		if (Target->GetPoint() == TestPoint)
		{
			return true;
		}
	}

	return false;
}

bool APlayer::IsCollider(FTileVector _Direction)
{
	return true == IsPixelCollider(_Direction) || true == IsComponentCollider(_Direction);
}
