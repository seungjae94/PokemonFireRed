#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class AMap;

enum class EPlayerState
{
	Idle,
	Walk,
	Jump,		// Ledge를 넘을 때
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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// FSM (Finite State Machine)
	void StateUpdate(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Walk(float _DeltaTime);
	void Jump(float _DeltaTime);

	// 충돌 체크
	bool CheckCollision();
	bool CheckJump();
private:
	UImageRenderer* Renderer = nullptr;

	// 월드 좌표
	FIntPoint Direction = FIntPoint::Down;
	FVector PrevPos = FVector::Zero;
	FVector NextPos = FVector::Zero;

	// 지면
	AMap* Map = nullptr;
	
	// 상태 관리
	EPlayerState State = EPlayerState::Idle;

	// 걷기 애니메이션 프레임당 재생 시간
	float WalkInterval = 1.0f / 6;
	float JumpInterval = WalkInterval / 8;

	// 이동 관련
	float speed = 3.6f;
	float WalkTime = 1.0f / speed; // 1칸 걷는데 걸리는 시간
	float JumpTime = 2.0f / speed; // 2칸 점프하는데 걸리는 시간
	bool IsMoving = false;
};

