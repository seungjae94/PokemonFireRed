#pragma once
#include <string>
#include <vector>
#include <random>
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineDebug.h>
#include "Global.h"

enum class EDirection
{
	Down,
	Up,
	Left,
	Right,
	None
};

template <typename A, typename B, typename C>
struct FTriple
{
public:
	A first;
	B second;
	C third;
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
		: X(std::lround(_Vector.X) / Global::TileSize), Y(std::lround(_Vector.Y) / Global::TileSize)
	{
	}

	// 단항 연산자
	FTileVector operator-()
	{
		return FTileVector(-X, -Y);
	}

	FTileVector& operator=(const FTileVector& _Other)
	{
		X = _Other.X;
		Y = _Other.Y;
		return *this;
	}

	bool operator==(const FTileVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	bool operator!=(const FTileVector& _Other) const
	{
		return X != _Other.X || Y != _Other.Y;
	}

	FTileVector operator+(const FTileVector& _Other) const
	{
		FTileVector NewPoint = *this;
		NewPoint.X += _Other.X;
		NewPoint.Y += _Other.Y;
		return NewPoint;
	}

	FTileVector& operator+=(const FTileVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}

	FTileVector operator-(const FTileVector& _Other) const
	{
		return FTileVector(X - _Other.X, Y - _Other.Y);
	}

	FTileVector operator*(int _Value) const
	{
		return FTileVector(X * _Value, Y * _Value);
	}

	bool operator<(const FTileVector& _Other) const
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

	bool operator>(const FTileVector& _Other) const
	{
		return (*this != _Other) && !(*this < _Other);
	}

	FVector ToFVector() const
	{
		return FVector(Global::TileSize * X, Global::TileSize * Y);
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

	static float Lerp(float _Start, float _End, float _t);
	static FVector Lerp(const FVector& _Start, const FVector& _End, float _t);
	static FVector Lerp(const FTileVector& _Start, const FTileVector& _End, float _t);
	static FTileVector ProjectToTileVector(const FVector& _Vec);
	static float Distance(const FVector& _Vec1, const FVector& _Vec2);
	
	static int Max(int _Left, int _Right)
	{
		if (_Left >= _Right)
		{
			return _Left;
		}
		return _Right;
	}

	static int Min(int _Left, int _Right)
	{
		if (_Left >= _Right)
		{
			return _Right;
		}
		return _Left;
	}

	static int Mod(int _Value, int _Divisor)
	{
		if (_Divisor <= 0)
		{
			MsgBoxAssert("Mod 함수는 0 이하의 수로 나눈 나머지를 지원하지 않습니다. " 
				+ std::to_string(_Value) +"를 " + std::to_string(_Divisor) + "로 나누려고 했습니다.");
			return 0;
		}

		if (_Value >= 0)
		{
			return _Value % _Divisor;
		}

		return (_Value % _Divisor + _Divisor) % _Divisor;
	}

	static int Floor(float _Value)
	{
		return std::lround(std::floor(_Value));
	}

	static int Round(float _Value)
	{
		return std::lround(_Value);
	}

	static int Pow(int _Value, int _Exp)
	{
		if (_Exp < 0)
		{
			MsgBoxAssert("반환 값이 int인 Pow 함수는 음의 지수를 지원하지 않습니다.");
			return 0;
		}

		int Result = 1;
		for (int i = 0; i < _Exp; i++)
		{
			Result *= _Value;
		}

		return Result;
	}
	
	// Reference: https://stackoverflow.com/questions/4915462/how-should-i-do-floating-point-comparison
	static bool Equal(float _Left, float _Right)
	{
		if (_Left == _Right)
		{
			return true;
		}

		float Diff = std::abs(_Left - _Right);
		float Norm = std::abs(_Left) + std::abs(_Right);

		return Diff < std::max<float>(FLT_MIN, 128 * FLT_EPSILON * Norm);
	}
 
	/// <param name="_Start">Inclusive</param>
	/// <param name="_End">Inclusive</param>
	static int RandomInt(int _Start, int _End);
	static float Random(float _Start, float _End);

	static int Cap(int _Value, int _Min, int _Max)
	{
		if (_Min > _Max)
		{
			MsgBoxAssert("Cap 조건이 잘못되었습니다. _Min < _Max입니다.");
			return _Value;
		}

		if (_Value < _Min)
		{
			return _Min;
		}
		else if (_Value > _Max)
		{
			return _Max;
		}
		
		return _Value;
	}

protected:

private:
	static std::random_device RandomDevice;
	static std::mt19937 RandomGenerator;
};

