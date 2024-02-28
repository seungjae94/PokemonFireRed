#include "AnimationElement.h"
#include "PokemonUtil.h"

AAnimationElement::AAnimationElement()
{
}

AAnimationElement::~AAnimationElement()
{
}

void AAnimationElement::CreateAnimation(std::string_view _AnimName, std::string_view _ImageName, int _Start, int _End, float _Interval, bool _Loop)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
		Renderer->SetImage(_ImageName);
		UPokemonUtil::AlignImage(Renderer, PivotType);
	}

	Renderer->CreateAnimation(_AnimName, _ImageName, _Start, _End, _Interval, _Loop);
}

void AAnimationElement::SetAnimation(std::string_view _AnimName)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert("AAnimationElement에 렌더러가 존재하지 않아 애니메이션을 재생할 수 없습니다.");
		return;
	}

	Renderer->ChangeAnimation(_AnimName);
}
