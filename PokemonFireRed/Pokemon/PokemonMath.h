#pragma once
#include <string>
#include <vector>
#include <EngineBase/EngineMath.h>
#include "Global.h"

enum class EDirection
{
	Down,
	Up,
	Left,
	Right,
	None
};

struct FTileVector
{
public:
	const static FTileVector Zero;
	const static FTileVector Up;
	const static FTileVector Down;
	const static FTileVector Left;
	const static FTileVector Right;

	int X = 0;
	int Y = 0;

	FTileVector()
	{
	}

	FTileVector(int _X, int _Y)
		: X(_X), Y(_Y)
	{
	}

	FTileVector(const FVector& _Vector)
		: X(std::lround(_Vector.X) / Global::TILE_SIZE), Y(std::lround(_Vector.Y) / Global::TILE_SIZE)
	{
	}

	// 단항 연산자
	FTileVector operator -()
	{
		return FTileVector(-X, -Y);
	}

	FTileVector& operator =(const FTileVector& _Other)
	{
		X = _Other.X;
		Y = _Other.Y;
		return *this;
	}

	bool operator ==(const FTileVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool operator !=(const FTileVector& _Other) const
	{
		return X != _Other.X || Y != _Other.Y;
	}

	FTileVector operator +(const FTileVector& _Other) const
	{
		FTileVector NewPoint = *this;
		NewPoint.X += _Other.X;
		NewPoint.Y += _Other.Y;
		return NewPoint;
	}

	FTileVector& operator +=(const FTileVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	FTileVector operator *(int _Value) const
	{
		return FTileVector(X * _Value, Y * _Value);
	}

	bool operator <(const FTileVector& _Other) const
	{
		if (X == _Other.X)
		{
			if (Y == _Other.Y)
			{
				return false;
			}

			return Y < _Other.Y;
		}

		return X < _Other.X;
	}

	bool operator >(const FTileVector& _Other) const
	{
		return (*this != _Other) && !(*this < _Other);
	}

	FVector ToFVector() const
	{
		return FVector(Global::TILE_SIZE * X, Global::TILE_SIZE * Y);
	}

	std::string ToDirectionString() const
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

	static std::vector<std::string> AllDirectionNames()
	{
		return {"Down", "Up", "Left", "Right"};
	}

	std::string ToString() const
	{
		return "[X : " + std::to_string(X) + " Y : " + std::to_string(Y) + "]";
	}
};

class UPokemonMath
{
public:
	// constructor destructor
	UPokemonMath();
	~UPokemonMath();

	// delete Function
	UPokemonMath(const UPokemonMath& _Other) = delete;
	UPokemonMath(UPokemonMath&& _Other) noexcept = delete;
	UPokemonMath& operator=(const UPokemonMath& _Other) = delete;
	UPokemonMath& operator=(UPokemonMath&& _Other) noexcept = delete;

	static FVector Lerp(const FVector& _Start, const FVector& _End, float _t);

protected:

private:

};

