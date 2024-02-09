#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class UEventHelper;

enum class EMovableState
{
	Idle,
	Walk
};

// �̺�Ʈ�� ���� ������ �̵��ϴ� ����
class AMovable : public AActor
{
	friend UEventHelper;
public:
	// constructor destructor
	AMovable();
	~AMovable();

	// delete Function
	AMovable(const AMovable& _Other) = delete;
	AMovable(AMovable&& _Other) noexcept = delete;
	AMovable& operator=(const AMovable& _Other) = delete;
	AMovable& operator=(AMovable&& _Other) noexcept = delete;

	void ChangeAnimation(EMovableState _State, const FTileVector& _Direction);

	virtual void SetTilePoint(const FTileVector& _Point)
	{
		SetActorLocation(_Point.ToFVector());
	}

	FTileVector GetDirection() const
	{
		return Direction;
	}

	void SetDirection(const FTileVector& _Direction)
	{
		Direction = _Direction;
	}

	void SetMoveState(EMovableState _MoveState)
	{
		MoveState = _MoveState;
	}

protected:
	// �̵� ���� ����
	FTileVector Direction = FTileVector::Down;
	FVector PrevPos;
	FVector NextPos;
	EMovableState MoveState = EMovableState::Idle;
	
	// ������
	UImageRenderer* Renderer = nullptr;
private:
};

