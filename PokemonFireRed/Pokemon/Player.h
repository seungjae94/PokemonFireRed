#pragma once
#include "PokemonMath.h"
#include "EventTarget.h"

class AMap;
class UMapLevel;

enum class EPlayerState
{
	None,
	Idle,
	Walk,
	WalkInPlace,
	Jump,
	Event
};

// 사용자 입력에 따라 이동하는 액터
class APlayer : public AEventTarget
{
public:
	// constructor destructor
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void SetMap(AMap* _Map)
	{
		Map = _Map;
	}

	// FSM (Finite State Machine)
	void StateChange(EPlayerState _State, bool _Restart = false);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void ChangeAnimation(EPlayerState _State, FTileVector _Direction);
	
	void IdleStart(bool _ResetAnimation = true);
	void Idle(float _DeltaTime);

	void WalkStart(bool _ResetAnimation = true);
	void Walk(float _DeltaTime);

	void WalkInPlaceStart(bool _ResetAnimation = true);
	void WalkInPlace(float _DeltaTime);

	void JumpStart(bool _ResetAnimation = true);
	void Jump(float _DeltaTime);

	// 충돌 체크
	bool IsLedge(FTileVector _Direction);
	bool IsCollider(FTileVector _Direction);
private:
	// 지면
	AMap* Map = nullptr;
	
	// 상태
	EPlayerState State = EPlayerState::None;

	// 이동 관련 변수
	FTileVector MemoryDirection = FTileVector::Zero;
	
	// 원작과 비슷한 걷기 속도는 3.6f
	float WalkSpeed = Global::CHARACTER_WALK_SPEED;
	float JumpSpeed = Global::CHARACTER_JUMP_SPEED;

	float IdleTime = 0.05f;  // Idle 상태를 유지하는 최소 시간
	float CurIdleTime = IdleTime;
	float WalkTime = 1.0f / WalkSpeed;
	float CurWalkTime = WalkTime;
	float WalkInPlaceTime = 0.05f; // WalkInPlace 상태를 유지하는 최소 시간
	float CurWalkInPlaceTime = WalkTime; 
	float JumpTime = 1.0f / JumpSpeed; // 2칸 점프하는데 걸리는 시간
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.9f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.
	float JumpInputLatency = 0.95f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.
};

