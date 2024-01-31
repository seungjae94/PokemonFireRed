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
	// 게임 기본 설정
	MainWindow.SetWindowScale({ 720, 480 });
	SetFrame(60);
	
	// 레벨 설정
	CreateLevel<UTitleLevel>("TitleLevel");
	ChangeLevel("TitleLevel");
}
