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

	// �̵� ����
	FTileVector Direction = FTileVector::Down;
	FTileVector MemoryDirection = FTileVector::Zero;
	FVector PrevPos;
	FVector NextPos;

	// ����
	AMap* Map = nullptr;
	
	// ���� ����
	EPlayerState State = EPlayerState::Idle;

	// �ȱ� �ִϸ��̼� �����Ӵ� ��� �ð�
	float WalkInterval = 1.0f / 6;
	float JumpInterval = WalkInterval / 8;

	// �̵� ����
	float speed = 3.6f;
	float WalkTime = 1.0f / speed; // 1ĭ �ȴµ� �ɸ��� �ð�
	float CurWalkTime = WalkTime;
	float JumpTime = 3.0f / speed; // 2ĭ �����ϴµ� �ɸ��� �ð�
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.9f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
	float JumpInputLatency = 0.95f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
};

