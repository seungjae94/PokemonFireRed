#pragma once
#include "EngineMath.h"

// 위치와 크기를 멤버로 갖고 있는 구조체
struct FTransform
{
public:
	// constructor destructor
	FTransform();
	FTransform(const FVector& _Pos, const FVector& _Scale)
		: Position(_Pos), Scale(_Scale)
	{
	}
	~FTransform();

	// delete Function
	//FTransform(const FTransform& _Other) = delete;
	//FTransform(FTransform&& _Other) noexcept = delete;
	//FTransform& operator=(const FTransform& _Other) = delete;
	//FTransform& operator=(FTransform&& _Other) noexcept = delete;

public:
	FVector GetScale() const
	{
		return Scale;
	}

	void SetScale(FVector _Value)
	{
		Scale = _Value;
	}

	FVector GetPosition() const
	{
		return Position;
	}

	void SetPosition(FVector _Value)
	{
		Position = _Value;
	}

	void AddPosition(FVector _Value)
	{
		Position += _Value;
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
		return static_cast<int>(Left());
	}

	int iRight() const
	{
		return static_cast<int>(Right());
	}

	int iTop() const
	{
		return static_cast<int>(Top());
	}

	int iBottom() const
	{
		return static_cast<int>(Bottom());
	}

protected:

private:
	FVector Scale;
	FVector Position;
};

