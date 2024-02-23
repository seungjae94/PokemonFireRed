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
	
	for (APokemonText* Text : Texts)
	{
		Text->SetActive(_Active, _ActiveTime);
	}

	for (AScrollBar* Bar : Bars)
	{
		Bar->SetActive(_Active, _ActiveTime);
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
	Texts.push_back(Text);
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
	Bars.push_back(Bar);
	return Bar;
}

