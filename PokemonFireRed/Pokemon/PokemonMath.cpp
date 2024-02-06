#include "PokemonMath.h"

const FTileVector FTileVector::Zero = { 0, 0 };
const FTileVector FTileVector::Up = { 0, -1 };
const FTileVector FTileVector::Down = { 0, 1 };
const FTileVector FTileVector::Left = { -1, 0 };
const FTileVector FTileVector::Right = { 1, 0 };

PokemonMath::PokemonMath()
{
}

PokemonMath::~PokemonMath()
{
}

FVector PokemonMath::Lerp(const FVector& _Start, const FVector& _End, float _t)
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
