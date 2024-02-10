#include "PokemonCore.h"
#include "TitleLevel.h"
#include "ExteriorPalletTownLevel.h"
#include "InteriorOaksLabLevel.h"
#include "InteriorPlayersHouse1FLevel.h"
#include "InteriorPlayersHouse2FLevel.h"
#include "InteriorRivalsHouseLevel.h"
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
	MainWindow.SetClearColor(Color8Bit::BlackA);

	// ���� ����
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");
	CreateLevel<UInteriorOaksLabLevel>("InteriorOaksLabLevel");
	CreateLevel<UInteriorPlayersHouse1FLevel>("InteriorPlayersHouse1FLevel");
	CreateLevel<UInteriorPlayersHouse2FLevel>("InteriorPlayersHouse2FLevel");
	CreateLevel<UInteriorRivalsHouseLevel>("InteriorRivalsHouseLevel");

	// ���� ���� ����
	//ChangeLevel("TitleLevel");					// ���� ������ �� ������ ����
	//ChangeLevel("InteriorPlayersHouse2FLevel");   // �� ���� ���� ���� ��ġ 

	ChangeLevel("ExteriorPalletTownLevel");
}
