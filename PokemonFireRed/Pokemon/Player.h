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

	// �浹 üũ
	bool IsLedge(FTileVector _Direction);
	bool IsCollider(FTileVector _Direction);
private:
	UImageRenderer* Renderer = nullptr;

	// ����
	AMap* Map = nullptr;
	
	// ����
	EPlayerState State = EPlayerState::Idle;

	// �ִϸ��̼� 1������ ��� �ð�
	float WalkInterval = 1.0f / 6;
	float JumpInterval = 1.0f / 48;

	// �̵� ���� ����
	FTileVector Direction = FTileVector::Down;
	FTileVector MemoryDirection = FTileVector::Zero;
	FVector PrevPos;
	FVector NextPos;

	// ���۰� ����� �ȱ� �ӵ��� 3.6f
	float WalkSpeed = 6.4f;
	float JumpSpeed = 1.2f;

	float IdleTime = 0.05f; // Idle ���¸� �����ϴ� �ּ� �ð�
	float CurIdleTime = IdleTime;
	float WalkTime = 1.0f / WalkSpeed; // 1ĭ �ȴµ� �ɸ��� �ð�
	float CurWalkTime = WalkTime;
	float WalkInPlaceTime = 0.05f; // WalkInPlace ���¸� �����ϴ� �ּ� �ð�
	float CurWalkInPlaceTime = WalkTime; 
	float JumpTime = 1.0f / JumpSpeed; // 2ĭ �����ϴµ� �ɸ��� �ð�
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.9f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
	float JumpInputLatency = 0.95f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
};

