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

class UEventTargetInitialSetting
{
	friend UEventManager;
public:
	UEventTargetInitialSetting(
		std::string_view _Name,
		const FTileVector& _Point = FTileVector::Zero,
		const FTileVector& _Direction = FTileVector::Zero,
		bool _Collidable = false,
		bool _Rotatable = false,
		bool _Walkable = false,
		bool _HasImage = false,
		std::string_view _ImageName = ""
	)
		: Name(UEngineString::ToUpper(_Name)),
		Point(_Point),
		Direction(_Direction),
		Collidable(_Collidable),
		Rotatable(_Rotatable),
		Walkable(_Walkable),
		HasImage(_HasImage),
		ImageName(UEngineString::ToUpper(_ImageName))
	{
		if (true == _HasImage && ImageName == "")
		{
			if (true == _Rotatable)
			{
				ImageName = Name + UEngineString::ToUpper("Idle.png");
			}
			else
			{
				ImageName = Name + UEngineString::ToUpper(".png");
			}
		}
	}
	
private:
	std::string Name;
	FTileVector Point = FTileVector::Zero;
	FTileVector Direction = FTileVector::Zero;
	bool Collidable = false;
	bool Rotatable = false;
	bool Walkable = false;
	bool HasImage = false;
	std::string ImageName;
};

// 이벤트에 의해 강제로 행동할 수 있는 액터
// - ex) 플레이어, NPC, 표지판, 바위 등
class AEventTarget : public AActor
{
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

	void ChangeAnimation(ETargetMoveState _State, const FTileVector& _Direction);

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
	// 이동 관련 변수
	FTileVector Point = FTileVector::Zero;
	FTileVector Direction = FTileVector::Down;
	ETargetMoveState MoveState = ETargetMoveState::Idle;
	float MoveTime = 0.0f;
	float Timer = 0.0f;
	int MoveIndex = -1;
	bool Collidable = false;
	bool Rotatable = false;
	bool Walkable = false;
	bool HasImage = false;
	bool IsExecutingMovingLogic = false;

	// 렌더러
	UImageRenderer* UpperBodyRenderer = nullptr;
	UImageRenderer* LowerBodyRenderer = nullptr;

	// 콜리전
	UCollision* Collision = nullptr;
private:
};

