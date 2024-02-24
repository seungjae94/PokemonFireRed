#include "Page.h"
#include "PokemonText.h"
#include "ScrollBar.h"
#include "PokemonUtil.h"

APage::APage()
{
}

APage::~APage()
{
}

void APage::SetActive(bool _Active, float _ActiveTime)
{
	AActor::SetActive(_Active, _ActiveTime);
	
	for (std::pair<UImageRenderer* const, std::list<AUIElement*>>& Pair : Elements)
	{
		UImageRenderer* const Container = Pair.first;
		std::list<AUIElement*>& List = Pair.second;

		if (nullptr == Container)
		{
			MsgBoxAssert("UI 엘리먼트의 컨테이너가 nullptr입니다.");
			return;
		}

		for (AUIElement* Element : List)
		{
			Element->SetActive(_Active, _ActiveTime);
		}
	}
}

APokemonText* APage::CreateText(
	UImageRenderer* _Container, 
	std::wstring _Text, 
	EPivotType _PivotType, 
	EAlignType _AlignType, 
	int _RelativePixelX, int _RelativePixelY, 
	EFontColor _Color, EFontSize _Size
)
{
	APokemonText* Text = GetWorld()->SpawnActor<APokemonText>();
	Text->Container = _Container;
	Text->PivotType = _PivotType;
	Text->AlignType = _AlignType;
	Text->Color = _Color;
	Text->Size = _Size;
	Text->SetText(_Text);
	Text->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Elements[_Container].push_back(Text);
	return Text;
}

AScrollBar* APage::CreateScrollBar(
	UImageRenderer* _Container, 
	EScrollType _ScrollType, 
	int _CurValue, int _MaxValue, 
	EPivotType _PivotType, 
	int _RelativePixelX, int _RelativePixelY
)
{
	AScrollBar* Bar = GetWorld()->SpawnActor<AScrollBar>();
	Bar->Container = _Container;
	Bar->ScrollType = _ScrollType;
	Bar->PivotType = _PivotType;
	Bar->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Bar->SetMaxValue(_MaxValue);
	Bar->SetValue(_CurValue);
	Elements[_Container].push_back(Bar);
	return Bar;
}

ACursor* APage::CreateCursor(UImageRenderer* _Container, int _Cursor, int _OptionCount, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, int _PixelGap)
{
	ACursor* Cursor = GetWorld()->SpawnActor<ACursor>();
	Cursor->Container = _Container;
	Cursor->PivotType = _PivotType;
	Cursor->PixelGap = _PixelGap;
	Cursor->OptionCount = _OptionCount;
	Cursor->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Cursor->SetCursor(_Cursor);
	Elements[_Container].push_back(Cursor);
	return Cursor;
}

APokemonIcon* APage::CreatePokemonIcon(UImageRenderer* _Container, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY)
{
	APokemonIcon* Icon = GetWorld()->SpawnActor<APokemonIcon>();
	Icon->Container = _Container;
	Icon->PivotType = _PivotType;
	Icon->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Elements[_Container].push_back(Icon);
	return Icon;
}

void APage::PlaceImage(UImageRenderer* _Renderer, EPivotType _PivotType, int _ScreenPixelX, int _ScreenPixelY)
{
	_Renderer->CameraEffectOff();

	FVector RenderScale = UPokemonUtil::GetRenderScale(_Renderer);

	FVector ZeroPos;
	switch (_PivotType)
	{
	case EPivotType::LeftTop:
		ZeroPos = RenderScale.Half2D();
		break;
	case EPivotType::LeftBot:
		ZeroPos = RenderScale.Half2D() + FVector(0.0f, Global::ScreenY - RenderScale.Y);
		break;
	case EPivotType::RightTop:
		ZeroPos = RenderScale.Half2D() + FVector(Global::FloatScreenX - RenderScale.X, 0.0f);
		break;
	case EPivotType::RightBot:
		ZeroPos = RenderScale.Half2D() + FVector(Global::FloatScreenX - RenderScale.X, Global::FloatScreenY - RenderScale.Y);
		break;
	case EPivotType::CenterTop:
		ZeroPos = FVector(Global::FloatHalfScreenX, RenderScale.hY());
		break;
	case EPivotType::CenterBot:
		ZeroPos = FVector(Global::FloatHalfScreenX, RenderScale.hY() + Global::FloatScreenY - RenderScale.Y);
		break;
	default:
		break;
	}

	FVector ScreenPos = ZeroPos + UPokemonUtil::PixelVector(_ScreenPixelX, _ScreenPixelY);
	_Renderer->SetTransform({ ScreenPos, RenderScale });
}

void APage::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (false == ContainerElementSync)
	{
		return;
	}

	for (std::pair<UImageRenderer* const, std::list<AUIElement*>>& Pair : Elements)
	{
		UImageRenderer* const Container = Pair.first;
		std::list<AUIElement*>& List = Pair.second;

		if (nullptr == Container)
		{
			MsgBoxAssert("UI 엘리먼트의 컨테이너가 nullptr입니다.");
			return;
		}

		for (AUIElement* Element : List)
		{
			Element->SetActive(Container->IsActive());
			Element->FollowContainer();
		}
	}
}

