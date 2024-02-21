#include "PokemonMath.h"

const FTileVector FTileVector::Zero = { 0, 0 };
const FTileVector FTileVector::Up = { 0, -1 };
const FTileVector FTileVector::Down = { 0, 1 };
const FTileVector FTileVector::Left = { -1, 0 };
const FTileVector FTileVector::Right = { 1, 0 };

std::random_device UPokemonMath::RandomDevice;
std::mt19937 UPokemonMath::RandomGenerator(UPokemonMath::RandomDevice());

UPokemonMath::UPokemonMath()
{
}

UPokemonMath::~UPokemonMath()
{
}

FVector UPokemonMath::Lerp(const FVector& _Start, const FVector& _End, float _t)
{
	if (_t <= 0.0f)
	{
		return _Start;
	}

	if (_t >= 1.0f)
	{
		return _End;
	}

	return _Start * (1 - _t) + _End * _t;
}

FVector UPokemonMath::Lerp(const FTileVector& _Start, const FTileVector& _End, float _t)
{
	return Lerp(_Start.ToFVector(), _End.ToFVector(), _t);
}

FTileVector UPokemonMath::ProjectToTileVector(const FVector& _Vec)
{
	FVector Vec = _Vec;
	float Size = Vec.Size2D();
	float MinDist = 2 * Size;

	std::vector<FVector> TestVectors = {
		FVector::Left * std::abs(Vec.X), 
		FVector::Right * std::abs(Vec.X),
		FVector::Up * std::abs(Vec.Y),
		FVector::Down * std::abs(Vec.Y)
	};

	FTileVector Result = FTileVector::Zero;
	for (FVector& TestVector : TestVectors)
	{
		float Dist = UPokemonMath::Distance(TestVector, Vec);

		if (Dist <= MinDist)
		{
			MinDist = Dist;
			Result = TestVector.Normalize2DReturn() * Global::FloatTileSize;
		}
	}

	return Result;
}

float UPokemonMath::Distance(const FVector& _Vec1, const FVector& _Vec2)
{
	float Dx = (_Vec1.X - _Vec2.X);
	float Dy = (_Vec1.Y - _Vec2.Y);

	return std::sqrtf(Dx*Dx + Dy*Dy);
}

int UPokemonMath::RandomInt(int _Start, int _End)
{
	std::uniform_int_distribution<int> Dist(_Start, _End);
	return Dist(RandomGenerator);
}

float UPokemonMath::Random(float _Start, float _End)
{
	std::uniform_real_distribution<float> Dist(_Start, _End);
	return Dist(RandomGenerator);
}