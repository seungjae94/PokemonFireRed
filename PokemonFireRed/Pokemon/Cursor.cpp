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
	Cursor = _Cursor;

	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
		Renderer->SetActive(IsActive());
	}

	Renderer->SetImage(ImageName);
	UPokemonUtil::AlignImage(Renderer, PivotType);
}

bool ACursor::IsLast() const
{
	return Cursor == OptionCount - 1;
}

void ACursor::FollowParentPosition()
{
	AUIElement::FollowParentPosition();

	FVector ActorPos = GetActorLocation();

	if (CursorPositions.size() <= Cursor)
	{
		SetActorLocation(ActorPos + UPokemonUtil::PixelVector(0, PixelGap * Cursor));
	}
	else
	{
		SetActorLocation(ActorPos + CursorPositions[Cursor]);
	}
}
