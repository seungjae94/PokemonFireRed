#include "AnimationElement.h"
#include "PokemonUtil.h"

AAnimationElement::AAnimationElement()
{
}

AAnimationElement::~AAnimationElement()
{
}

void AAnimationElement::CreateAnimation(std::string_view _AnimName, int _Start, int _End, float _Interval, bool _Loop)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
		Renderer->SetImage(ImageName);
		UPokemonUtil::AlignImage(Renderer, PivotType);
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Start, _End, _Interval, _Loop);
}

void AAnimationElement::CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
		Renderer->SetImage(ImageName);
		UPokemonUtil::AlignImage(Renderer, PivotType);
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Indexs, _Times, _Loop);
}

void AAnimationElement::SetAnimation(std::string_view _AnimName)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert("AAnimationElement�� �������� �������� �ʾ� �ִϸ��̼��� ����� �� �����ϴ�.");
		return;
	}

	Renderer->ChangeAnimation(_AnimName);
}
