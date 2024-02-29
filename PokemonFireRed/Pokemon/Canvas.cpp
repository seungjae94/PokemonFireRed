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
	return Cursor;
}

APokemonElement* ACanvas::CreatePokemonElement(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, EPokemonElementType _ElementType)
{
	APokemonElement* Icon = GetWorld()->SpawnActor<APokemonElement>();
	Icon->SetParent(_Parent);
	Icon->SetRenderingOrder(_Order);
	Icon->SetPivotType(_PivotType);
	Icon->SetElementType(_ElementType);
	Icon->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	return Icon;
}

AImageElement* ACanvas::CreateImageElement(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY)
{
	AImageElement* Image = GetWorld()->SpawnActor<AImageElement>();
	Image->SetParent(_Parent);
	Image->SetRenderingOrder(_Order);
	Image->SetPivotType(_PivotType);
	Image->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	return Image;
}


AAnimationElement* ACanvas::CreateAnimationElement(AUIParent* _Parent, ERenderingOrder _Order, EPivotType _PivotType, int _RelativePixelX, int _RelativePixelY, std::string_view _ImageName)
{
	AAnimationElement* Anim = GetWorld()->SpawnActor<AAnimationElement>();
	Anim->SetParent(_Parent);
	Anim->SetRenderingOrder(_Order);
	Anim->SetPivotType(_PivotType);
	Anim->SetImage(_ImageName);
	Anim->SetRelativePosition(UPokemonUtil::PixelVector(_RelativePixelX, _RelativePixelY));
	return Anim;
}