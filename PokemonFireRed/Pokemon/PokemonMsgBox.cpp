#include "PokemonMsgBox.h"

PokemonMsgBox::PokemonMsgBox() 
{
}

PokemonMsgBox::~PokemonMsgBox() 
{
}

void PokemonMsgBox::SetBackgroundImage(std::string_view _ImageName)
{
}

void PokemonMsgBox::SetTextColor(EFontColor _Color)
{
}

void PokemonMsgBox::SetWriteSpeed(int _WriteSpeed)
{
}

void PokemonMsgBox::SetLineSpace(int _LineSpace)
{
}

void PokemonMsgBox::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement();
}

void PokemonMsgBox::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);
}
