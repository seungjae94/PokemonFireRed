#pragma once
#include "EngineMath.h"

// 위치와 크기를 멤버로 갖고 있는 구조체
struct FTransform
{
public:
	FTransform();
	~FTransform();

public:
	FVector GetScale()
	{
		return Scale;
	}

	void SetScale(FVector _Value)
	{
		Scale = _Value;
	}

	FVector GetPosition()
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

protected:

private:
	FVector Scale;
	FVector Position;
};

