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
	// 게임 기본 설정
	MainWindow.SetWindowScale({ Global::SCREEN_X, Global::SCREEN_Y });
	MainWindow.SetClearColor(Color8Bit::BlackA);

	// 레벨 생성
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");
	CreateLevel<UInteriorOaksLabLevel>("InteriorOaksLabLevel");
	CreateLevel<UInteriorPlayersHouse1FLevel>("InteriorPlayersHouse1FLevel");
	CreateLevel<UInteriorPlayersHouse2FLevel>("InteriorPlayersHouse2FLevel");
	CreateLevel<UInteriorRivalsHouseLevel>("InteriorRivalsHouseLevel");

	// 시작 레벨 설정
	//ChangeLevel("TitleLevel");					// 실제 릴리즈 때 시작할 레벨
	//ChangeLevel("InteriorPlayersHouse2FLevel");   // 맵 레벨 최초 시작 위치 

	ChangeLevel("ExteriorPalletTownLevel");
}
