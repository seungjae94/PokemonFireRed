#include "Canvas.h"
#include "Text.h"
#include "ScrollBar.h"
#include "PokemonUtil.h"

ACanvas::ACanvas()
{
}

ACanvas::~ACanvas()
{
}

AText* ACanvas::CreateText(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, EAlignType _AlignType, EFontColor _Color, EFontSize _Size)
{
	AText* Text = GetWorld()->SpawnActor<AText>();
	Text->SetParent(_Parent);
	Text->SetRenderingOrder(_Order);
	Text->SetPivotType(_PivotType);
	Text->SetAlignType(_AlignType);
	Text->SetColor(_Color);
	Text->SetSize(_Size);
	Text->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	_Parent->AppendChild(Text);
	return Text;
}

AScrollBar* ACanvas::CreateScrollBar(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, EScrollType _ScrollType)
{
	AScrollBar* Bar = GetWorld()->SpawnActor<AScrollBar>();
	Bar->SetParent(_Parent);
	Bar->SetRenderingOrder(_Order);
	Bar->SetPivotType(_PivotType);
	Bar->SetScrollType(_ScrollType);
	Bar->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	_Parent->AppendChild(Bar);
	return Bar;
}

ACursor* ACanvas::CreateCursor(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, std::string _ImageName, int _PixelGap)
{
	ACursor* Cursor = GetWorld()->SpawnActor<ACursor>();
	Cursor->SetParent(_Parent);
	Cursor->SetRenderingOrder(_Order);
	Cursor->SetPivotType(_PivotType);
	Cursor->SetImage(_ImageName);
	Cursor->SetPixelGap(_PixelGap);
	Cursor->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	_Parent->AppendChild(Cursor);
	return Cursor;
}

AImageElement* ACanvas::CreateImageElement(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, EImageElementType _Type)
{
	AImageElement* Image = GetWorld()->SpawnActor<AImageElement>();
	Image->SetParent(_Parent);
	Image->SetRenderingOrder(_Order);
	Image->SetPivotType(_PivotType);
	Image->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	Image->SetType(_Type);
	_Parent->AppendChild(Image);
	return Image;
}

FTransform ACanvas::GetUITransform()
{
	return {Global::HalfScreen, Global::Screen};
}
