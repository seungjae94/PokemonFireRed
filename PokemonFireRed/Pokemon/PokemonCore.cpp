#include "PokemonCore.h"
#include "TitleLevel.h"
#include "TutorialLevel.h"
#include "ExteriorPalletTownLevel.h"
#include "InteriorOaksLabLevel.h"
#include "InteriorPlayersHouse1FLevel.h"
#include "InteriorPlayersHouse2FLevel.h"
#include "InteriorRivalsHouseLevel.h"
#include "Global.h"

UPokemonCore::UPokemonCore()
{
}

UPokemonCore::~UPokemonCore()
{
}

void UPokemonCore::BeginPlay()
{
	// 게임 기본 설정
	MainWindow.SetWindowScale({ Global::SCREEN_X, Global::SCREEN_Y });
	MainWindow.SetClearColor(Color8Bit::BlackA);


	// UI 리소스 로딩
	UEngineDirectory CurDir;
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("UI");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// 레벨 생성
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UTutorialLevel>("TutorialLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");
	CreateLevel<UInteriorOaksLabLevel>("InteriorOaksLabLevel");
	CreateLevel<UInteriorPlayersHouse1FLevel>("InteriorPlayersHouse1FLevel");
	CreateLevel<UInteriorPlayersHouse2FLevel>("InteriorPlayersHouse2FLevel");
	CreateLevel<UInteriorRivalsHouseLevel>("InteriorRivalsHouseLevel");

	// 시작 레벨 설정
	UEventManager::ChangeLevel("TitleLevel");					 // 실제 릴리즈 때 시작할 레벨
	//UEventManager::ChangeLevel("TutorialLevel");
	//UEventManager::ChangeLevel("InteriorPlayersHouse2FLevel");   // 맵 레벨 최초 시작 위치 
	//UEventManager::ChangeLevel("ExteriorPalletTownLevel");
}