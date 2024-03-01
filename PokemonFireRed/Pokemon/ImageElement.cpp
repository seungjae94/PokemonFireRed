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
	ImageName = _ImageName;

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

void AImageElement::CreateAnimation(std::string_view _AnimName, int _Start, int _End, float _Interval, bool _Loop)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data()  + "을 생성할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Start, _End, _Interval, _Loop);
}

void AImageElement::CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 생성할 수 없습니다.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Indexs, _Times, _Loop);
}

void AImageElement::ChangeAnimation(std::string_view _AnimName, bool _IsForce)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation 실패. 렌더러가 존재하지 않아 애니메이션 " + _AnimName.data() + "을 재생할 수 없습니다.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation 실패. 이미지를 지정하지 않아 애니메이션 " + _AnimName.data() + "을 재생할 수 없습니다.");
		return;
	}

	Renderer->ChangeAnimation(_AnimName, _IsForce);
}
