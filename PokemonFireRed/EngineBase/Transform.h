#pragma once
#include "EngineMath.h"

// ��ġ�� ũ�⸦ ����� ���� �ִ� ����ü
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

