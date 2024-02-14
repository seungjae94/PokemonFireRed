#pragma once
#include "EngineMath.h"

enum ECollisionType
{
	Point,
	CirCle,
	Rect
};

class CollisionFunctionInit;

// 위치와 크기를 멤버로 갖고 있는 구조체
class FTransform
{
	friend CollisionFunctionInit;

private:
	static bool (*CollisionFunction[static_cast<int>(Rect)][static_cast<int>(Rect)])(const FTransform& _Left, const FTransform& _Right);
public:
	// constructor destructor
	FTransform();
	FTransform(const FVector& _Pos, const FVector& _Scale)
		: Position(_Pos), Scale(_Scale)
	{

	}
	~FTransform();

	static bool CircleToCircle(const FTransform& _Left, const FTransform& _Right);

public:
	void SetScale(FVector _Value)
	{
		Scale = _Value;
	}
	FVector GetScale() const
	{
		return Scale;
	}
	void SetPosition(FVector _Value)
	{
		Position = _Value;
	}
	void AddPosition(FVector _Value)
	{
		Position += _Value;
	}
	FVector GetPosition() const
	{
		return Position;
	}

	float Left() const
	{
		return Position.X - Scale.hX();
	}
	float Right() const
	{
		return Position.X + Scale.hX();
	}
	float Top() const
	{
		return Position.Y - Scale.hY();
	}
	float Bottom() const
	{
		return Position.Y + Scale.hY();
	}

	int iLeft() const
	{
		return std::lround(Left());
	}
	int iRight() const
	{
		return std::lround(Right());
	}
	int iTop() const
	{
		return std::lround(Top());
	}
	int iBottom() const
	{
		return std::lround(Bottom());
	}

	bool Collision(ECollisionType _ThisType, ECollisionType _OtherType, const FTransform& _Other);
protected:

private:
	FVector Scale;
	FVector Position;
};


