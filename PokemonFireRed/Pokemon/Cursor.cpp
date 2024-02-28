#include "Cursor.h"
#include "PokemonUtil.h"

ACursor::ACursor()
{
}

ACursor::~ACursor()
{
}

void ACursor::SetCursor(int _Cursor)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	Renderer->SetImage(ImageName);
	UPokemonUtil::AlignImage(Renderer, PivotType);

	Cursor = _Cursor;
}

bool ACursor::IsLast() const
{
	return Cursor == OptionCount - 1;
}

void ACursor::FollowContainer()
{
	AUIElement::FollowContainer();

	FVector AbsolutePos = Pivot + RelativePos;
	AbsolutePos += UPokemonUtil::PixelVector(0, PixelGap * Cursor);
	SetActorLocation(AbsolutePos);
}
