#include "Player.h"
#include "PokemonInput.h"
#include "Map.h"
#include "Global.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventManager.h"
#include "MenuCanvas.h"
#include "Pokemon.h"
#include "SoundManager.h"

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
	UpdateInputState(_DeltaTime);
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

bool APlayer::HasControl() const
{
	return State != EPlayerState::OutOfControl;
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
		UpperAniName += "SlowWalk" + DirectionStr + Global::SuffixUpperBody + std::to_string(FootOrder);
		LowerAniName += "SlowWalk" + DirectionStr + Global::SuffixLowerBody + std::to_string(FootOrder);
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
}

void APlayer::Idle(float _DeltaTime)
{
	// 0. ȸ������ ��� �ٸ� �Է��� �����Ѵ�.
	if (true == IsRotating)
	{
		CurRotateTime -= _DeltaTime;

		if (CurRotateTime < 0.0f)
		{
			IsRotating = false;
			ChangeAnimation(EPlayerState::Idle, Direction);

			FTileVector KeyPressDirection = UPokemonInput::GetPressDirection();
			if (Direction == KeyPressDirection)
			{
				// ȸ���� �����µ��� ���� ����Ű�� ������ �ִٸ� �ȱ⸦ �õ��Ѵ�.
				TryWalk();
			}
			else
			{
				// ���� �̵����� ���� �߻��ϴ� �̺�Ʈ�� ó���Ѵ�.

				// e.g. ǥ���� �տ��� ���� ���鼭 ���ִٰ� ǥ���� �� ����Ű�� ������ ��� 
				bool ReadResult = TryReadEvent();
				if (true == ReadResult)
				{
					return;
				}

				// e.g. �� �տ��� ���� ���鼭 ���ִٰ� �� �� ����Ű�� ������ ���
				bool ArrowClickResult = TryArrowClickEvent();
				if (true == ArrowClickResult)
				{
					return;
				}
			}
		}
		return;
	}

	// 1. Z ��ư�� ������.
	if (true == IsZDown() && true == TryZClickEvent())
	{
		return;
	}

	// 2. ���� Ű�� ������.
	if (true == IsEnterDown())
	{
		UEventManager::OpenMenuWindow();
		return;
	}

	// 3. ���� ���� �ִ� ����� ���� ����Ű�� ������ �ִ�.
	// -> �̺�Ʈ�� Ȯ���Ѵ�.
	// -> �浹�� Ȯ���Ѵ�.
	// -> �̺�Ʈ�� �浹�� ���ٸ� ������ �ȴ´�.
	FTileVector KeyPressDirection = UPokemonInput::GetPressDirection();
	if (KeyPressDirection == Direction)
	{
		TryWalk();
		return;
	}

	// 4. �ƹ� Ű�� ������ �ʾҴٸ� �ƹ� �ϵ� �Ͼ�� �ʴ´�.
	if (KeyPressDirection == FTileVector::Zero)
	{
		return;
	}

	// 5. ���� ���� �ִ� ����� �ٸ� ����Ű�� ������ �ִ�.
	// -> �ϴ� ���⸸ �ٲ۴�.
	if (KeyPressDirection != Direction)
	{
		IsRotating = true;
		Direction = KeyPressDirection;
		CurRotateTime = RotateTime;
		ChangeAnimation(EPlayerState::Walk, Direction);
		return;
	}

}

void APlayer::TryWalk()
{
	// e.g. ǥ���� �տ��� ǥ������ ���� ���ִٰ� ǥ���� �� ����Ű�� ������ ��� 
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// e.g. �� �տ��� ���� ���� ���ִٰ� �� �� ����Ű�� ������ ���
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// �տ� Ledge�� �ִ� ���
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// �տ� �浹ü�� �ִ� ���
	if (true == IsCollider(Direction))
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// �̺�Ʈ�� �߻����� �ʰ�, �տ� Ledge�� ����, �浹ü�� ���� ���
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
	// 0. ���� �̵� ���̴�.
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
			MemoryDirection = UPokemonInput::GetPressDirection();
		}

		return;
	}
	IsExecutingMovingLogic = false;
	UEventManager::SetPoint(GetWorld()->GetName(), GetName(), NextPoint);

	// 1. Read �̺�Ʈ�� �߻��ߴ�.
	// e.g. ǥ������ �ٶ󺸸鼭 ǥ���� ������ �̵��ߴ�.
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// 2. StepOn �̺�Ʈ�� �߻��ߴ�.
	bool StepOnResult = TryStepOnEvent();
	if (true == StepOnResult)
	{
		if (State == EPlayerState::Walk)
		{
			StateChange(EPlayerState::Idle);
		}
		return;
	}

	// 3. Battle �̺�Ʈ�� �߻��ߴ�.
	bool BattleOccurResult = TryBattleEvent();
	if (true == BattleOccurResult)
	{
		return;
	}

	// 4. ����ϰ� �ִ� �Է� ������ ����. ��, ����Ű�� ������ �ʾҴ�.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}
	Direction = MemoryDirection;

	// 5. ArrowClick �̺�Ʈ�� �߻��ߴ�. 
	// - �Է� ������ �ֱ� ������ ArrowClick �׼��� ��ȿ�ϴ�.
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// 6. �Է� ���⿡ Ledge�� �ִ�.
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 7. �Է� ���⿡ �浹ü�� �ִ�.
	if (true == IsCollider(Direction))
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 8. ����Ű�� ������ ������ �̺�Ʈ�� �浹�� �߻����� �ʾҴ�.
	// -> ��� �ȴ´�.
	WalkStart();
}

void APlayer::WalkInPlaceStart()
{
	ChangeAnimation(EPlayerState::WalkInPlace, Direction);
}

void APlayer::WalkInPlace(float _DeltaTime)
{
	// 1. Z ��ư�� ������.
	if (true == IsZDown() && true == TryZClickEvent())
	{
		return;
	}

	// 2. ���� Ű�� ������.
	if (true == IsEnterDown())
	{
		UEventManager::OpenMenuWindow();
		return;
	}

	// 3. ����Ű�� ������ ���� �ʴ�.
	FTileVector InputDirection = UPokemonInput::GetPressDirection();
	if (InputDirection == FTileVector::Zero)
	{
		// �ִϸ��̼� ����� �����ٸ� �ٷ� Idle ���·� �����Ѵ�.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			StateChange(EPlayerState::Idle);
		}
		return;
	}

	// 2. ���� ���� �ִ� ����� �Է� ������ ����.
	if (InputDirection == Direction)
	{
		// �ִϸ��̼Ǹ� �����Ѵ�.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			WalkInPlaceStart();
		}
		return;
	}

	Direction = InputDirection;
	// 3. Read �̺�Ʈ�� �߻��ߴ�.
	if (true == TryReadEvent())
	{
		return;
	}

	// 4. ArrowClick �̺�Ʈ�� �߻��ߴ�.
	if (true == TryArrowClickEvent())
	{
		return;
	}

	// 5. �Է� ���⿡ Ledge�� �ִ�.
	if (IsLedge(InputDirection))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 6. �Է� ���⿡ �浹ü�� �ִ�.
	if (true == IsCollider(InputDirection))
	{
		// �ִϸ��̼Ǹ� �����Ѵ�.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			WalkInPlaceStart();
		}
		return;
	}

	// 7. �� ���� ���
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
	// 0. ���� ���� ���̴�.
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
			MemoryDirection = UPokemonInput::GetPressDirection();
		}

		return;
	}
	bool IsExecutingMovingLogic = false;
	UEventManager::SetPoint(GetWorld()->GetName(), GetName(), NextPoint);

	// 1. Read �̺�Ʈ�� �߻��ߴ�.
	// e.g. ǥ���� ������ �����ߴ�.	
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// 2. StepOn �̺�Ʈ�� �߻��ߴ�.
	bool StepOnResult = TryStepOnEvent();
	if (true == StepOnResult)
	{
		return;
	}

	// 3. ��Ʋ �̺�Ʈ�� �߻��ߴ�.
	bool BattleOccurResult = TryBattleEvent();
	if (true == BattleOccurResult)
	{
		return;
	}

	// 4. ����ϰ� �ִ� �Է� ������ ����.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}
	Direction = MemoryDirection;

	// 5. ArrowClick �̺�Ʈ�� �߻��ߴ�. 
	// - �Է� ������ �ֱ� ������ ArrowClick �׼��� ��ȿ�ϴ�.
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// 6. �Է� ���⿡ Ledge�� �ִ�.
	if (IsLedge(Direction) == true)
	{
		JumpStart();
		return;
	}

	// 7. �Է� ���⿡ �浹ü�� �ִ�.
	if (true == IsCollider(Direction))
	{

		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 8. �� ���� ���
	StateChange(EPlayerState::Walk);
}

bool APlayer::IsZDown()
{
	return UEngineInput::IsDown('Z');
}

bool APlayer::IsEnterDown()
{
	return UEngineInput::IsDown(VK_RETURN);
}

bool APlayer::TryZClickEvent()
{
	// Ŭ�� �̺�Ʈ = �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ� and ZŰ �Է�
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = UEventManager::TriggerEvent(Trigger, EEventTriggerAction::ZClick);

	if (true == RunResult)
	{
		USoundManager::PlaySE(RN::SEClick);
	}

	return RunResult;
}

bool APlayer::TryReadEvent()
{
	// Read �̺�Ʈ = �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ�
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = UEventManager::TriggerEvent(Trigger, EEventTriggerAction::Read);

	return RunResult;
}

bool APlayer::TryArrowClickEvent()
{
	// Push �̺�Ʈ = �÷��̾ Ʈ���ſ� ���� and Ʈ���Ű� �ִ� ������ ����Ű �Է�
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = UEventManager::TriggerEvent(Trigger, EEventTriggerAction::ArrowClick);

	return RunResult;
}

bool APlayer::TryStepOnEvent()
{
	// StepOn �̺�Ʈ = �÷��̾ Ʈ���ſ� ���� ��ġ�� ����

	FTileVector CurPoint = FTileVector(GetActorLocation());

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(CurPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = UEventManager::TriggerEvent(Trigger, EEventTriggerAction::StepOn);

	return RunResult;
}

bool APlayer::TryBattleEvent()
{
	if (false == IsGrass())
	{
		return false;
	}

	Color8Bit CurPointColor = GetPointColor();
	int ZoneIndex = CurPointColor.R;

	int RandomNumber = UPokemonMath::RandomInt(0, 2879);
	int ZoneBoundNumber = 320;

	ZoneBoundNumber = UPokemonMath::Round(ZoneBoundNumber * Global::WildBattleFrequency);

	// ���� < 320 �� ���� ��Ʋ�� �����Ѵ�.
	if (RandomNumber >= ZoneBoundNumber)
	{
		return false;
	}

	const UWildPokemonZone* Zone = UGameDB::FindWildPokemonZone(GetWorld()->GetName(), ZoneIndex);
	FWildPokemonConstructorParam ConstructorParam = Zone->GenerateWildPokemonConstructorParam();
	UEventManager::WildBattle(ConstructorParam);
	return true;
}

Color8Bit APlayer::GetPointColor(FTileVector _RelativePoint)
{
	// ���� �̹��� �»���� �������� ����� Ÿ���� ��� ��ǥ
	FVector MapRelativeTargetPos = (GetActorLocation() - Map->GetActorLocation()) + _RelativePoint.ToFVector();
	Color8Bit Color = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPos.iX(),
		MapRelativeTargetPos.iY(),
		Color8Bit::WhiteA
	);
	return Color;
}

bool APlayer::IsLedge(FTileVector _Direction)
{
	Color8Bit Color = GetPointColor(_Direction);

	if (_Direction == FTileVector::Down)
	{
		return Color == Color8Bit(255, 255, 0, 0);
	}

	return false;
}

bool APlayer::IsGrass()
{
	Color8Bit Color = GetPointColor();
	return Color.G == 255 && Color.B == 0;
}

bool APlayer::IsPixelCollider(FTileVector _Direction)
{
	FVector MapRelativeCurPos = GetActorLocation() - Map->GetActorLocation();
	FVector MapRelativeTargetPos = MapRelativeCurPos + _Direction.ToFVector();

	Color8Bit CurColor = Map->GetCollisionImage()->GetColor(
		MapRelativeCurPos.iX(),
		MapRelativeCurPos.iY(),
		Color8Bit::MagentaA
	);

	Color8Bit TargetColor = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPos.iX(),
		MapRelativeTargetPos.iY(),
		Color8Bit::MagentaA
	);

	if (CurColor == Global::PixelColliderBlockUpDown)
	{
		if (_Direction == FTileVector::Up || _Direction == FTileVector::Down)
		{
			return true;
		}
	}

	if (TargetColor == Global::PixelColliderBlockUpDown)
	{
		if (-_Direction == FTileVector::Up || -_Direction == FTileVector::Down)
		{
			return true;
		}
	}

	return TargetColor == Global::PixelColliderBlock || (TargetColor.R == 255 && TargetColor.G == 255);
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



void APlayer::UpdateInputState(float _DeltaTime)
{
	InputStatus.SinceLastZKeyDown += _DeltaTime;
	InputStatus.SinceLastArrowKeyDown += _DeltaTime;

	InputStatus.ZKeyDown = UEngineInput::IsDown('Z');
	if (true == InputStatus.ZKeyDown)
	{
		InputStatus.SinceLastZKeyDown = 0.0f;
	}

	if (UEngineInput::IsDown(VK_DOWN))
	{
		InputStatus.ArrowKey = VK_DOWN;
	}
	else if (UEngineInput::IsDown(VK_UP))
	{
		InputStatus.ArrowKey = VK_UP;
	}
	else if (UEngineInput::IsDown(VK_LEFT))
	{
		InputStatus.ArrowKey = VK_LEFT;
	}
	else if (UEngineInput::IsDown(VK_RIGHT))
	{
		InputStatus.ArrowKey = VK_RIGHT;
	}
	else
	{
		InputStatus.ArrowKey = 0;
	}

	if (InputStatus.ArrowKey != 0)
	{
		InputStatus.ArrowKeyDown = true;
		InputStatus.SinceLastArrowKeyDown = 0.0f;
	}
}
