#include "PlayerCharacter.h"
#include "PokemonInput.h"
#include "Map.h"
#include "Global.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventManager.h"
#include "MenuCanvas.h"
#include "Pokemon.h"
#include "SoundManager.h"

APlayerCharacter::APlayerCharacter()
{
}

APlayerCharacter::~APlayerCharacter()
{
}

void APlayerCharacter::BeginPlay()
{
	AEventTarget::BeginPlay();
}

void APlayerCharacter::Tick(float _DeltaTime)
{
	AEventTarget::Tick(_DeltaTime);
	UpdateInputState(_DeltaTime);
	StateUpdate(_DeltaTime);
}

void APlayerCharacter::StateUpdate(float _DeltaTime)
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

void APlayerCharacter::StateChange(EPlayerState _State, bool _Restart)
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
		CollisionSETimer = 0.0f;
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

bool APlayerCharacter::HasControl() const
{
	return State != EPlayerState::OutOfControl;
}

void APlayerCharacter::ChangeAnimation(EPlayerState _State, FTileVector _Direction)
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

void APlayerCharacter::IdleStart()
{
	ChangeAnimation(EPlayerState::Idle, Direction);
}

void APlayerCharacter::Idle(float _DeltaTime)
{
	// 0. 회전중일 경우 다른 입력은 무시한다.
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
				// 회전이 끝났는데도 현재 방향키를 누르고 있다면 걷기를 시도한다.
				TryWalk();
			}
			else
			{
				// 방향 이동으로 인해 발생하는 이벤트를 처리한다.

				// e.g. 표지판 앞에서 옆을 보면서 서있다가 표지판 쪽 방향키를 누르는 경우 
				bool ReadResult = TryReadEvent();
				if (true == ReadResult)
				{
					return;
				}

				// e.g. 문 앞에서 옆을 보면서 서있다가 문 쪽 방향키를 누르는 경우
				bool ArrowClickResult = TryArrowClickEvent();
				if (true == ArrowClickResult)
				{
					return;
				}
			}
		}
		return;
	}

	// 1. Z 버튼을 눌렀다.
	if (true == IsZDown() && true == TryZClickEvent())
	{
		return;
	}

	// 2. 엔터 키를 눌렀다.
	if (true == IsEnterDown())
	{
		UEventManager::OpenMenuWindow();
		return;
	}

	// 3. 지금 보고 있는 방향과 같은 방향키를 누르고 있다.
	// -> 이벤트를 확인한다.
	// -> 충돌을 확인한다.
	// -> 이벤트도 충돌도 없다면 앞으로 걷는다.
	FTileVector KeyPressDirection = UPokemonInput::GetPressDirection();
	if (KeyPressDirection == Direction)
	{
		TryWalk();
		return;
	}

	// 4. 아무 키도 누르지 않았다면 아무 일도 일어나지 않는다.
	if (KeyPressDirection == FTileVector::Zero)
	{
		return;
	}

	// 5. 지금 보고 있는 뱡향과 다른 방향키를 누르고 있다.
	// -> 일단 방향만 바꾼다.
	if (KeyPressDirection != Direction)
	{
		IsRotating = true;
		Direction = KeyPressDirection;
		CurRotateTime = RotateTime;
		ChangeAnimation(EPlayerState::Walk, Direction);
		return;
	}

}

void APlayerCharacter::TryWalk()
{
	// e.g. 표지판 앞에서 표지판을 보고 서있다가 표지판 쪽 방향키를 누르는 경우 
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// e.g. 문 앞에서 문을 보고 서있다가 문 쪽 방향키를 누르는 경우
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// 앞에 Ledge가 있는 경우
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 앞에 충돌체가 있는 경우
	if (true == IsCollider(Direction))
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 이벤트도 발생하지 않고, 앞에 Ledge도 없고, 충돌체도 없는 경우
	StateChange(EPlayerState::Walk);
}

void APlayerCharacter::WalkStart()
{
	ChangeAnimation(EPlayerState::Walk, Direction);
	MemoryDirection = FTileVector::Zero;
	CurWalkTime = WalkTime;
	PrevPoint = FTileVector(GetActorLocation());
	NextPoint = PrevPoint + Direction;
}

void APlayerCharacter::Walk(float _DeltaTime)
{
	// 0. 아직 이동 중이다.
	if (CurWalkTime > 0.0f)
	{
		IsExecutingMovingLogic = true;

		// 이동 로직
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

	// 1. Read 이벤트가 발생했다.
	// e.g. 표지판을 바라보면서 표지판 앞으로 이동했다.
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// 2. StepOn 이벤트가 발생했다.
	bool StepOnResult = TryStepOnEvent();
	if (true == StepOnResult)
	{
		if (State == EPlayerState::Walk)
		{
			StateChange(EPlayerState::Idle);
		}
		return;
	}

	// 3. Battle 이벤트가 발생했다.
	bool BattleOccurResult = TryBattleEvent();
	if (true == BattleOccurResult)
	{
		return;
	}

	// 4. 기억하고 있는 입력 방향이 없다. 즉, 방향키를 누르지 않았다.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}
	Direction = MemoryDirection;

	// 5. ArrowClick 이벤트가 발생했다. 
	// - 입력 방향이 있기 때문에 ArrowClick 액션이 유효하다.
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// 6. 입력 방향에 Ledge가 있다.
	if (IsLedge(Direction) == true)
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 7. 입력 방향에 충돌체가 있다.
	if (true == IsCollider(Direction))
	{
		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 8. 방향키를 누르고 있지만 이벤트도 충돌도 발생하지 않았다.
	// -> 계속 걷는다.
	WalkStart();
}

void APlayerCharacter::WalkInPlaceStart()
{
	ChangeAnimation(EPlayerState::WalkInPlace, Direction);
}

void APlayerCharacter::WalkInPlace(float _DeltaTime)
{
	CollisionSETimer -= _DeltaTime;

	// 1. Z 버튼을 눌렀다.
	if (true == IsZDown() && true == TryZClickEvent())
	{
		return;
	}

	// 2. 엔터 키를 눌렀다.
	if (true == IsEnterDown())
	{
		UEventManager::OpenMenuWindow();
		return;
	}

	// 3. 방향키를 누르고 있지 않다.
	FTileVector InputDirection = UPokemonInput::GetPressDirection();
	if (InputDirection == FTileVector::Zero)
	{
		// 애니메이션 재생이 끝났다면 바로 Idle 상태로 돌입한다.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			StateChange(EPlayerState::Idle);
		}
		return;
	}

	// 2. 지금 보고 있는 방향과 입력 방향이 같다.
	if (InputDirection == Direction)
	{
		if (CollisionSETimer <= 0.0f)
		{
			USoundManager::PlaySE(RN::SECollision);
			CollisionSETimer = CollisionSEInterval;
		}

		// 애니메이션만 갱신한다.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			WalkInPlaceStart();
		}
		return;
	}

	Direction = InputDirection;
	// 3. Read 이벤트가 발생했다.
	if (true == TryReadEvent())
	{
		return;
	}

	// 4. ArrowClick 이벤트가 발생했다.
	if (true == TryArrowClickEvent())
	{
		return;
	}

	// 5. 입력 방향에 Ledge가 있다.
	if (IsLedge(InputDirection))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 6. 입력 방향에 충돌체가 있다.
	if (true == IsCollider(InputDirection))
	{
		// 애니메이션만 갱신한다.
		if (true == LowerBodyRenderer->IsCurAnimationEnd())
		{
			WalkInPlaceStart();
		}
		return;
	}

	// 7. 그 외의 경우
	StateChange(EPlayerState::Walk);
}

void APlayerCharacter::JumpStart()
{
	USoundManager::PlaySE(RN::SELedgeJump);
	ChangeAnimation(EPlayerState::Jump, Direction);
	MemoryDirection = FTileVector::Zero;
	CurJumpTime = JumpTime;
	PrevPoint = FTileVector(GetActorLocation());
	NextPoint = PrevPoint + Direction.ToFVector() * 2;
}

void APlayerCharacter::Jump(float _DeltaTime)
{
	// 0. 아직 점프 중이다.
	if (CurJumpTime > 0.0f)
	{
		bool IsExecutingMovingLogic = true;

		// 이동 로직
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

	// 1. Read 이벤트가 발생했다.
	// e.g. 표지판 앞으로 점프했다.	
	bool ReadResult = TryReadEvent();
	if (true == ReadResult)
	{
		return;
	}

	// 2. StepOn 이벤트가 발생했다.
	bool StepOnResult = TryStepOnEvent();
	if (true == StepOnResult)
	{
		return;
	}

	// 3. 배틀 이벤트가 발생했다.
	bool BattleOccurResult = TryBattleEvent();
	if (true == BattleOccurResult)
	{
		return;
	}

	// 4. 기억하고 있는 입력 방향이 없다.
	if (MemoryDirection == FTileVector::Zero)
	{
		StateChange(EPlayerState::Idle);
		return;
	}
	Direction = MemoryDirection;

	// 5. ArrowClick 이벤트가 발생했다. 
	// - 입력 방향이 있기 때문에 ArrowClick 액션이 유효하다.
	bool ArrowClickResult = TryArrowClickEvent();
	if (true == ArrowClickResult)
	{
		return;
	}

	// 6. 입력 방향에 Ledge가 있다.
	if (IsLedge(Direction) == true)
	{
		JumpStart();
		return;
	}

	// 7. 입력 방향에 충돌체가 있다.
	if (true == IsCollider(Direction))
	{

		StateChange(EPlayerState::WalkInPlace);
		return;
	}

	// 8. 그 외의 경우
	StateChange(EPlayerState::Walk);
}

bool APlayerCharacter::IsZDown()
{
	return UEngineInput::IsDown('Z');
}

bool APlayerCharacter::IsEnterDown()
{
	return UEngineInput::IsDown(VK_RETURN);
}

bool APlayerCharacter::TryZClickEvent()
{
	// 클릭 이벤트 = 플레이어가 트리거에 인접 and 트리거를 바라봄 and Z키 입력
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = Trigger->TriggerEvent(EEventTriggerAction::ZClick);

	if (true == RunResult)
	{
		USoundManager::PlaySE(RN::SEClick);
	}

	return RunResult;
}

bool APlayerCharacter::TryReadEvent()
{
	// Read 이벤트 = 플레이어가 트리거에 인접 and 트리거를 바라봄
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = Trigger->TriggerEvent(EEventTriggerAction::Read);

	return RunResult;
}

bool APlayerCharacter::TryArrowClickEvent()
{
	// Push 이벤트 = 플레이어가 트리거에 인접 and 트리거가 있는 방향의 방향키 입력
	FTileVector CurPoint = FTileVector(GetActorLocation());
	FTileVector TargetPoint = CurPoint + Direction;

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(TargetPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = Trigger->TriggerEvent(EEventTriggerAction::ArrowClick);

	return RunResult;
}

bool APlayerCharacter::TryStepOnEvent()
{
	// StepOn 이벤트 = 플레이어가 트리거와 같은 위치에 있음

	FTileVector CurPoint = FTileVector(GetActorLocation());

	AEventTrigger* Trigger = UEventManager::FindCurLevelTriggerAt<AEventTrigger>(CurPoint);

	if (nullptr == Trigger)
	{
		return false;
	}

	bool RunResult = Trigger->TriggerEvent(EEventTriggerAction::StepOn);

	return RunResult;
}

bool APlayerCharacter::TryBattleEvent()
{
	if (false == IsGrass())
	{
		return false;
	}

	Color8Bit CurPointColor = GetPointColor();
	int ZoneIndex = CurPointColor.R;

	int RandomNumber = UPokemonMath::RandomInt(0, 2879);
	int ZoneBoundNumber = 320;

	ZoneBoundNumber = UPokemonMath::Round(ZoneBoundNumber * Global::EncounterRateBonusCoeff);

	// 난수 < 320 일 때만 배틀을 시작한다.
	if (RandomNumber >= ZoneBoundNumber)
	{
		return false;
	}

	const UWildPokemonZone* Zone = UGameData::FindWildPokemonZone(GetWorld()->GetName(), ZoneIndex);
	FWildPokemonConstructorParam ConstructorParam = Zone->GenerateWildPokemonConstructorParam();
	UEventManager::WildBattle(ConstructorParam);
	return true;
}

Color8Bit APlayerCharacter::GetPointColor(FTileVector _RelativePoint)
{
	// 맵을 이미지 좌상단을 기준으로 계산한 타겟의 상대 좌표
	FVector MapRelativeTargetPos = (GetActorLocation() - Map->GetActorLocation()) + _RelativePoint.ToFVector();
	Color8Bit Color = Map->GetCollisionImage()->GetColor(
		MapRelativeTargetPos.iX(),
		MapRelativeTargetPos.iY(),
		Color8Bit::WhiteA
	);
	return Color;
}

bool APlayerCharacter::IsLedge(FTileVector _Direction)
{
	Color8Bit Color = GetPointColor(_Direction);

	if (_Direction == FTileVector::Down)
	{
		return Color == Color8Bit(255, 255, 0, 0);
	}

	return false;
}

bool APlayerCharacter::IsGrass()
{
	Color8Bit Color = GetPointColor();
	return Color.G == 255 && Color.B == 0;
}

bool APlayerCharacter::IsPixelCollider(FTileVector _Direction)
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

bool APlayerCharacter::IsComponentCollider(FTileVector _Direction)
{
	std::vector<UCollision*> CollisionResult;

	bool IsCollided = Collision->CollisionCheck(ECollisionOrder::NPC, CollisionResult);

	FTileVector TestPoint = GetPoint() + _Direction;
	for (UCollision* Collision : CollisionResult)
	{
		AEventTarget* Target = dynamic_cast<AEventTarget*>(Collision->GetOwner());

		if (nullptr == Target)
		{
			MsgBoxAssert("콜리전을 갖고 있지만 이벤트 타겟이 아닌 오브젝트" + Collision->GetOwner()->GetName() + "가 존재합니다.");
			return false;
		}

		if (Target->GetPoint() == TestPoint)
		{
			return true;
		}
	}

	return false;
}

bool APlayerCharacter::IsCollider(FTileVector _Direction)
{
	return true == IsPixelCollider(_Direction) || true == IsComponentCollider(_Direction);
}



void APlayerCharacter::UpdateInputState(float _DeltaTime)
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
