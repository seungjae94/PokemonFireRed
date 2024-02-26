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
		Renderer = CreateImageRenderer(ERenderingOrder::Text);
		Renderer->CameraEffectOff();
	}

	Renderer->SetImage(ImageName);
	UPokemonUtil::AlignImage(Renderer, PivotType);

	Cursor = _Cursor;
}

void ACursor::EnableCancel(int _CancelPixelX, int _CancelPixelY)
{
	HasCancel = true;
	CancelPixelX = _CancelPixelX;
	CancelPixelY = _CancelPixelY;
}

bool ACursor::IsCancel()
{
	if (false == HasCancel)
	{
		return false;
	}

	return Cursor == OptionCount - 1;
}

void ACursor::FollowContainer()
{
	AUIElement::FollowContainer();

	FVector AbsolutePos = Pivot + RelativePos;
	AbsolutePos += UPokemonUtil::PixelVector(0, PixelGap * Cursor);
	SetActorLocation(AbsolutePos);
}
