#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class UEventHelper;

enum class EMovableState
{
	Idle,
	Walk
};

// 이벤트에 의해 강제로 이동하는 액터
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
	// 이동 관련 변수
	FTileVector Direction = FTileVector::Down;
	FVector PrevPos;
	FVector NextPos;
	EMovableState MoveState = EMovableState::Idle;
	
	// 렌더러
	UImageRenderer* Renderer = nullptr;
private:
};

