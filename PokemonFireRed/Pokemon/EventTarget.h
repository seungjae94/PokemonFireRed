#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class UEventManager;

enum class ETargetMoveState
{
	Idle,
	Walk
};

// �̺�Ʈ�� ���� ������ �ൿ�� �� �ִ� ����
// - ex) �÷��̾�, NPC, ǥ����, ���� ��
class AEventTarget : public AActor
{
	friend UEventManager;
public:
	// constructor destructor
	AEventTarget();
	~AEventTarget();

	// delete Function
	AEventTarget(const AEventTarget& _Other) = delete;
	AEventTarget(AEventTarget&& _Other) noexcept = delete;
	AEventTarget& operator=(const AEventTarget& _Other) = delete;
	AEventTarget& operator=(AEventTarget&& _Other) noexcept = delete;

	void ChangeAnimation(ETargetMoveState _State, const FTileVector& _Direction);

	FTileVector GetDirection() const
	{
		return Direction;
	}

	void SetDirection(const FTileVector& _Direction)
	{
		Direction = _Direction;
	}

	void SetMoveState(ETargetMoveState _MoveState)
	{
		MoveState = _MoveState;
	}

	ETargetMoveState GetMoveState() const
	{
		return MoveState;
	}

protected:
	// �̵� ���� ����
	FTileVector Direction = FTileVector::Down;
	FVector PrevPos;
	FVector NextPos;
	ETargetMoveState MoveState = ETargetMoveState::Idle;
	float MoveTime = 0.0f;
	float Timer = 0.0f;
	int MoveIndex = -1;
	bool Rotatable = false;
	
	// ������
	UImageRenderer* Renderer = nullptr;
private:
};

