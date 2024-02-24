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
		Renderer->SetImage(RN::BlackCursor);
		FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
		Renderer->SetTransform({ RenderScale.Half2D(), RenderScale });
	}

	Cursor = _Cursor;
}

void ACursor::FollowContainer()
{
	AUIElement::FollowContainer();

	FVector AbsolutePos = Pivot + RelativePos;
	AbsolutePos += UPokemonUtil::PixelVector(0, PixelGap * Cursor);
	SetActorLocation(AbsolutePos);
}
