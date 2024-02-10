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

// ����� �Է¿� ���� �̵��ϴ� ����
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

	// �浹 üũ
	bool IsLedge(FTileVector _Direction);
	bool IsCollider(FTileVector _Direction);
private:
	// ����
	AMap* Map = nullptr;
	
	// ����
	EPlayerState State = EPlayerState::None;

	// �̵� ���� ����
	FTileVector MemoryDirection = FTileVector::Zero;
	
	// ���۰� ����� �ȱ� �ӵ��� 3.6f
	float WalkSpeed = Global::CHARACTER_WALK_SPEED;
	float JumpSpeed = Global::CHARACTER_JUMP_SPEED;

	float IdleTime = 0.05f;  // Idle ���¸� �����ϴ� �ּ� �ð�
	float CurIdleTime = IdleTime;
	float WalkTime = 1.0f / WalkSpeed;
	float CurWalkTime = WalkTime;
	float WalkInPlaceTime = 0.05f; // WalkInPlace ���¸� �����ϴ� �ּ� �ð�
	float CurWalkInPlaceTime = WalkTime; 
	float JumpTime = 1.0f / JumpSpeed; // 2ĭ �����ϴµ� �ɸ��� �ð�
	float CurJumpTime = JumpTime;
	float WalkInputLatency = 0.9f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
	float JumpInputLatency = 0.95f; // �ȱ� ������ �� �ۼ�Ʈ�� �������� ������ �Է��� �ޱ� ������ ������.
};

