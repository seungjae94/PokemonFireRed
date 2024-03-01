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
		MsgBoxAssert(GetName() + "::CreateAnimation ����. �������� �������� �ʾ� �ִϸ��̼� " + _AnimName.data()  + "�� ������ �� �����ϴ�.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation ����. �̹����� �������� �ʾ� �ִϸ��̼� " + _AnimName.data() + "�� ������ �� �����ϴ�.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Start, _End, _Interval, _Loop);
}

void AImageElement::CreateAnimation(std::string_view _AnimName, const std::vector<int>& _Indexs, std::vector<float> _Times, bool _Loop)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation ����. �������� �������� �ʾ� �ִϸ��̼� " + _AnimName.data() + "�� ������ �� �����ϴ�.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::CreateAnimation ����. �̹����� �������� �ʾ� �ִϸ��̼� " + _AnimName.data() + "�� ������ �� �����ϴ�.");
		return;
	}

	Renderer->CreateAnimation(_AnimName, ImageName, _Indexs, _Times, _Loop);
}

void AImageElement::ChangeAnimation(std::string_view _AnimName, bool _IsForce)
{
	if (nullptr == Renderer)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation ����. �������� �������� �ʾ� �ִϸ��̼� " + _AnimName.data() + "�� ����� �� �����ϴ�.");
		return;
	}

	if ("" == ImageName)
	{
		MsgBoxAssert(GetName() + "::ChangeAnimation ����. �̹����� �������� �ʾ� �ִϸ��̼� " + _AnimName.data() + "�� ����� �� �����ϴ�.");
		return;
	}

	Renderer->ChangeAnimation(_AnimName, _IsForce);
}
