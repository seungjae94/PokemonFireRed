#include "Page.h"
#include "Text.h"
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

AText* APage::CreateText(
	UImageRenderer* _Container, 
	std::wstring _Text, 
	EPivotType _PivotType, 
	EAlignType _AlignType, 
	int _RelativePixelX, int _RelativePixelY, 
	EFontColor _Color, EFontSize _Size,
	ERenderingOrder _Order
)
{
	AText* Text = GetWorld()->SpawnActor<AText>();
	Text->Container = _Container;
	Text->PivotType = _PivotType;
	Text->AlignType = _AlignType;
	Text->Color = _Color;
	Text->Size = _Size;
	Text->SetText(_Text);
	Text->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Text->SetRenderingOrder(_Order);
	Elements[_Container].push_back(Text);
	return Text;
}

AScrollBar* APage::CreateScrollBar(
	UImageRenderer* _Container, 
	EScrollType _ScrollType, 
	int _CurValue, int _MaxValue, 
	EPivotType _PivotType, 
	int _RelativePixelX, int _RelativePixelY,
	ERenderingOrder _Order
)
{
	AScrollBar* Bar = GetWorld()->SpawnActor<AScrollBar>();
	Bar->Container = _Container;
	Bar->ScrollType = _ScrollType;
	Bar->PivotType = _PivotType;
	Bar->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Bar->SetMaxValue(_MaxValue);
	Bar->SetValue(_CurValue);
	Bar->SetRenderingOrder(_Order);
	Elements[_Container].push_back(Bar);
	return Bar;
}

ACursor* APage::CreateCursor(
	UImageRenderer* _Container, std::string _ImageName,
	int _Cursor, int _OptionCount, 
	EPivotType _PivotType, 
	int _RelativePixelX, int _RelativePixelY, int _PixelGap,
	ERenderingOrder _Order
)
{
	ACursor* Cursor = GetWorld()->SpawnActor<ACursor>();
	Cursor->Container = _Container;
	Cursor->ImageName = _ImageName;
	Cursor->PivotType = _PivotType;
	Cursor->PixelGap = _PixelGap;
	Cursor->OptionCount = _OptionCount;
	Cursor->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Cursor->SetRenderingOrder(_Order);
	Cursor->SetCursor(_Cursor);
	Elements[_Container].push_back(Cursor);
	return Cursor;
}

APokemonElement* APage::CreatePokemonElement(
	UImageRenderer* _Container, 
	EPokemonElementType _ElementType,
	EPivotType _PivotType, 
	int _RelativePixelX, int _RelativePixelY,
	ERenderingOrder _Order
)
{
	APokemonElement* Icon = GetWorld()->SpawnActor<APokemonElement>();
	Icon->Container = _Container;
	Icon->Type = _ElementType;
	Icon->PivotType = _PivotType;
	Icon->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Icon->SetRenderingOrder(_Order);
	Elements[_Container].push_back(Icon);
	return Icon;
}

AImageElement* APage::CreateImageElement(
	UImageRenderer* _Container, 
	EPivotType _PivotType, 
	int _RelativePixelX, int _RelativePixelY,
	ERenderingOrder _Order
)
{
	AImageElement* Image = GetWorld()->SpawnActor<AImageElement>();
	Image->Container = _Container;
	Image->PivotType = _PivotType;
	Image->SetRelativePos(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Image->SetRenderingOrder(_Order);
	Elements[_Container].push_back(Image);
	return Image;
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

