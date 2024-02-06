#include "PokemonMath.h"

const FIntPoint FIntPoint::Zero = { 0, 0 };
const FIntPoint FIntPoint::Up = { 0, -1 };
const FIntPoint FIntPoint::Down = { 0, 1 };
const FIntPoint FIntPoint::Left = { -1, 0 };
const FIntPoint FIntPoint::Right = { 1, 0 };

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
