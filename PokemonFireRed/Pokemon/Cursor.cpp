#include "Cursor.h"

ACursor::ACursor()
{
}

ACursor::~ACursor()
{
}

void ACursor::FollowContainer()
{
	AUIElement::FollowContainer();

	FVector AbsolutePos = Pivot + RelativePos;
	AbsolutePos += UPokemonUtil::PixelVector(0, PixelGap * Cursor);
	SetActorLocation(AbsolutePos);
}
