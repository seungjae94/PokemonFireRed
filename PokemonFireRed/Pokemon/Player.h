#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class AMap;

enum class EPlayerState
{
	Idle,
	Walk,
	WalkInPlace,
	Jump,
};

class APlayer : public AActor
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

	void SetTilePoint(const FTileVector& _Point)
	{
		SetActorLocation(_Point.ToFVector());
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void StateChange(EPlayerState _State);
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
	UImageRenderer* Renderer = nullptr;

	// 지면
	AMap* Map = nullptr;
	
	// 상태
	EPlayerState State = EPlayerState::Idle;

	// 애니메이션 1프레임 재생 시간
	float WalkInterval = 1.0f / 6;
	float JumpInterval = 1.0f / 48;

	// 이동 관련 변수
	FTileVector Direction = FTileVector::Down;
	FTileVector MemoryDirection = FTileVector::Zero;
	FVector PrevPos;
	FVector NextPos;

	// 원작과 비슷한 걷기 속도는 3.6f
	float WalkSpeed = 6.4f;
	float JumpSpeed = 1.2f;

	float IdleTime = 0.05f; // Idle 상태를 유지하는 최소 시간
	float CurIdleTime = IdleTime;
	float WalkTime = 1.0f / WalkSpeed; // 1칸 걷는데 걸리는 시간
	float CurWalkTime = WalkTime;
	float WalkInPlaceTime = 0.05f; // WalkInPlace 상태를 유지하는 최소 시간
	float CurWalkInPlaceTime = WalkTime; 
	float JumpTime = 1.0f / JumpSpeed; // 2칸 점프하는데 걸리는 시간
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.9f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.
	float JumpInputLatency = 0.95f; // 걷기 동작을 몇 퍼센트나 수행했을 때부터 입력을 받기 시작할 것인지.
};

