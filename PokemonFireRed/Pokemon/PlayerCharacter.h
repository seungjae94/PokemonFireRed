#pragma once
#include "PokemonMath.h"
#include "EventTarget.h"

class AMap;
class UMapLevel;
class UEventManager;
class UEventProcessor;

enum class EPlayerState
{
	None,
	Idle,
	Walk,
	WalkInPlace,
	Jump,
	OutOfControl
};

enum class EPlayerAction
{
	ZClick,
	Read,
	ArrowClick,
	StepOn,
	Menu,
};

// 사용자 입력에 따라 이동하는 액터
class APlayerCharacter : public AEventTarget
{
	friend UEventManager;
	friend UEventProcessor;
public:
	// constructor destructor
	APlayerCharacter();
	~APlayerCharacter();

	// delete Function
	APlayerCharacter(const APlayerCharacter& _Other) = delete;
	APlayerCharacter(APlayerCharacter&& _Other) noexcept = delete;
	APlayerCharacter& operator=(const APlayerCharacter& _Other) = delete;
	APlayerCharacter& operator=(APlayerCharacter&& _Other) noexcept = delete;

	void SetMap(AMap* _Map)
	{
		Map = _Map;
	}

	// FSM (Finite State Machine)
	void StateChange(EPlayerState _State, bool _Restart = false);

	bool HasControl() const;

protected:
private:
	class UInputStatus
	{
	public:
		bool ZKeyDown = false;
		bool ArrowKeyDown = false;
		int ArrowKey = 0;
		float SinceLastZKeyDown = 100000000.0f;
		float SinceLastArrowKeyDown = 100000000.0f;
	};

	UInputStatus InputStatus;
	void UpdateInputState(float _DeltaTime);

	// 지면
	AMap* Map = nullptr;
	
	// 상태
	EPlayerState State = EPlayerState::None;

	// 이동 관련 변수
	FTileVector MemoryDirection = FTileVector::Zero;
	FTileVector PrevPoint;
	FTileVector NextPoint;
	
	// 원작과 비슷한 걷기 속도는 3.6f
	float WalkSpeed = Global::CharacterWalkSpeed;
	float JumpSpeed = Global::CharacterJumpSpeed;

	bool IsRotating = false;
	float RotateTime = 0.5f / WalkSpeed;
	float CurRotateTime = RotateTime;
	float WalkTime = 1.0f / WalkSpeed;
	float CurWalkTime = WalkTime;
	float JumpTime = 1.0f / JumpSpeed; // 2칸 점프하는데 걸리는 시간
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.75f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.
	float JumpInputLatency = 0.875f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.

	const float CollisionSEInterval = 2.0f / WalkSpeed;
	float CollisionSETimer = 0.0f;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void ChangeAnimation(EPlayerState _State, FTileVector _Direction);

	void IdleStart();
	void Idle(float _DeltaTime);
	void TryWalk();

	void WalkStart();
	void Walk(float _DeltaTime);

	void WalkInPlaceStart();
	void WalkInPlace(float _DeltaTime);

	void JumpStart();
	void Jump(float _DeltaTime);

	// 이벤트 체크 함수
	bool IsZDown();
	bool IsEnterDown();

	// 이벤트 실행 시도 함수
	bool TryZClickEvent();
	bool TryReadEvent();
	bool TryArrowClickEvent();
	bool TryStepOnEvent();
	bool TryBattleEvent();

	// 충돌 체크
	Color8Bit GetPointColor(FTileVector _RelativePoint = FTileVector::Zero); // 충돌맵에서 (CurPoint + _RelativePoint)의 색상
	bool IsLedge(FTileVector _Direction);
	bool IsGrass();
	//const FGrassInfo* GetGrassType();
	bool IsPixelCollider(FTileVector _Direction);
	bool IsComponentCollider(FTileVector _Direction);
	bool IsCollider(FTileVector _Direction);
};

