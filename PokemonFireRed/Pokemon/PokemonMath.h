#pragma once
#include <string>
#include <EngineBase/EngineMath.h>

struct FIntPoint
{
public:
	const static FIntPoint Zero;
	const static FIntPoint Up;
	const static FIntPoint Down;
	const static FIntPoint Left;
	const static FIntPoint Right;

	int X = 0;
	int Y = 0;

	FIntPoint()
	{
	}

	FIntPoint(int _X, int _Y)
		: X(_X), Y(_Y)
	{
	}

	bool operator ==(const FIntPoint& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool operator !=(const FIntPoint& _Other) const
	{
		return X != _Other.X || Y != _Other.Y;
	}

	FIntPoint operator +(const FIntPoint& _Other) const
	{
		FIntPoint NewPoint = *this;
		NewPoint.X += _Other.X;
		NewPoint.Y += _Other.Y;
		return NewPoint;
	}

	FIntPoint& operator +=(const FIntPoint& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}


	FVector ToFVector() const
	{
		return FVector(X, Y);
	}

	std::string ToString() const
	{
		if (*this == Up)
		{
			return "Up";
		}
		else if (*this == Down)
		{
			return "Down";
		}
		else if (*this == Left)
		{
			return "Left";
		}
		else if (*this == Right)
		{
			return "Right";
		}

		return "None";
	}
};

class PokemonMath
{
public:
	// constructor destructor
	PokemonMath();
	~PokemonMath();

	// delete Function
	PokemonMath(const PokemonMath& _Other) = delete;
	PokemonMath(PokemonMath&& _Other) noexcept = delete;
	PokemonMath& operator=(const PokemonMath& _Other) = delete;
	PokemonMath& operator=(PokemonMath&& _Other) noexcept = delete;

protected:

private:

};

