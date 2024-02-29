#include "ImageElement.h"
#include "PokemonUtil.h"

AImageElement::AImageElement()
{
}

AImageElement::~AImageElement()
{
}

void AImageElement::SetImage(std::string_view _ImageName)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	Renderer->SetImage(_ImageName);
	UPokemonUtil::AlignImage(Renderer, PivotType);
}

FTransform AImageElement::GetUITransform()
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
	}

	return Renderer->GetActorBaseTransform();
}
