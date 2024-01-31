#include "PokemonCore.h"
#include "TitleLevel.h"
#include "OutdoorLevel.h"

PokemonCore::PokemonCore()
{
}

PokemonCore::~PokemonCore()
{
}

void PokemonCore::BeginPlay()
{
	// ���� �⺻ ����
	MainWindow.SetWindowScale({ 720, 480 });
	SetFrame(60);
	
	// ���� ����
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UOutdoorLevel>("OutdoorLevel");

	// ���� ���� ����
	ChangeLevel("OutdoorLevel");
}
