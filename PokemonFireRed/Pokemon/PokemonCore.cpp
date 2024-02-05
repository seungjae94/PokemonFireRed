#include "PokemonCore.h"
#include "TitleLevel.h"
#include "ExteriorPalletTownLevel.h"
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

	// 레벨 생성
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");

	// 시작 레벨 설정
	ChangeLevel("TitleLevel");
}
