#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"

class UEventManager;
class UEventProcessor;
class PokemonDebug;

enum class ETargetMoveState
{
	Idle,
	Walk
};

class UEventTargetSetting
{
	friend UEventManager;
public:
	UEventTargetSetting()
	{
	}
	
	void SetName(std::string_view _Name)
	{
		Name = UEngineString::ToUpper(_Name);
	}

	void SetPoint(const FTileVector& _Point)
	{
		Point = _Point;
	}

	void SetDirection(const FTileVector& _Direction)
	{
		Direction = _Direction;
	}

	void SetCollidable(bool _Value)
	{
		Collidable = _Value;
	}

	void SetRotatable(bool _Value)
	{
		Rotatable = _Value;
	}

	void SetWalkable(bool _Value)
	{
		Walkable = _Value;
	}

	void SetImageNameAuto()
	{
		if (true == Rotatable)
		{
			ImageName = Name + UEngineString::ToUpper("Idle.png");
		}
		else
		{
			ImageName = Name + UEngineString::ToUpper(".png");
		}
	}

	void SetImageName(std::string_view _ImageName = "")
	{
		ImageName = UEngineString::ToUpper(_ImageName);
	}

	void SetHeight(int _Height)
	{
		Height = _Height;
	}

private:
	std::string Name = "";
	FTileVector Point = FTileVector::Zero;
	FTileVector Direction = FTileVector::Zero;
	bool Collidable = false;
	bool Rotatable = false;
	bool Walkable = false;
	std::string ImageName = "";
	int Height = 2;
};

// 이벤트에 의해 강제로 행동할 수 있는 액터
// - ex) 플레이어, NPC, 표지판, 바위 등
class AEventTarget : public AActor
{
	friend UEventProcessor;
	friend UEventManager;
	friend PokemonDebug;
public:
	// constructor destructor
	AEventTarget();
	~AEventTarget();

	// delete Function
	AEventTarget(const AEventTarget& _Other) = delete;
	AEventTarget(AEventTarget&& _Other) noexcept = delete;
	AEventTarget& operator=(const AEventTarget& _Other) = delete;
	AEventTarget& operator=(AEventTarget&& _Other) noexcept = delete;

	void ChangeMoveAnimation(ETargetMoveState _State, const FTileVector& _Direction);

	FTileVector GetPoint() const
	{
		return Point;
	}

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
	// 이벤트 타겟 초기 세팅
	FTileVector Point = FTileVector::Zero;
	FTileVector Direction = FTileVector::Down;
	bool Collidable = false;
	bool Rotatable = false;
	bool Walkable = false;
	bool HasImage = false;
	int Height = 2;

	// 이동 관련 멤버
	ETargetMoveState MoveState = ETargetMoveState::Idle;
	float MoveTime = 0.0f;
	float Timer = 0.0f;
	int MoveIndex = -1;
	char FootOrder = 0;		// 0 = 오른발이 나갈 차례, 1 = 왼발이 나갈 차례
	bool IsExecutingMovingLogic = false;

	void IncFootOrder()
	{
		FootOrder = (FootOrder + 1) % 2;
	}

	void ResetFootOrder()
	{
		FootOrder = 0;
	}


	// 렌더러
	UImageRenderer* UpperBodyRenderer = nullptr;
	UImageRenderer* LowerBodyRenderer = nullptr;
	std::string ImageName = "";

	// 콜리전
	UCollision* Collision = nullptr;
private:
};

