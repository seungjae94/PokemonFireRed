#include "PokemonCore.h"
#include "TitleLevel.h"
#include "OutdoorLevel.h"
#include "Global.h"

PokemonCore::PokemonCore()
{
}

PokemonCore::~PokemonCore()
{
}

void PokemonCore::BeginPlay()
{
	// ���� �⺻ ����
	MainWindow.SetWindowScale({ Global::SCREEN_X, Global::SCREEN_Y });
	
	// ���� ����
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UOutdoorLevel>("OutdoorLevel");

	// ���� ���� ����
	ChangeLevel("OutdoorLevel");
}
