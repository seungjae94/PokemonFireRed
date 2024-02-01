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
	MainWindow.SetWindowScale({ SCREEN_X, SCREEN_Y });
	SetFrame(FRAME_RATE);
	
	// ���� ����
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UOutdoorLevel>("OutdoorLevel");

	// ���� ���� ����
	ChangeLevel("OutdoorLevel");
}
