#include "PokemonCore.h"
#include "TitleLevel.h"

PokemonCore::PokemonCore()
{
}

PokemonCore::~PokemonCore()
{
}

void PokemonCore::BeginPlay()
{
	SetFrame(60);

	CreateLevel<UTitleLevel>("TitleLevel");

	ChangeLevel("TitleLevel");
}
