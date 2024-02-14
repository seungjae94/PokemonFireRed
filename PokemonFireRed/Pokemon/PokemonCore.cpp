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
	// ���� �⺻ ����
	MainWindow.SetWindowScale({ Global::SCREEN_X, Global::SCREEN_Y });
	MainWindow.SetClearColor(Color8Bit::BlackA);


	// UI ���ҽ� �ε�
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

	// ���� ����
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UTutorialLevel>("TutorialLevel");
	CreateLevel<UExteriorPalletTownLevel>("ExteriorPalletTownLevel");
	CreateLevel<UInteriorOaksLabLevel>("InteriorOaksLabLevel");
	CreateLevel<UInteriorPlayersHouse1FLevel>("InteriorPlayersHouse1FLevel");
	CreateLevel<UInteriorPlayersHouse2FLevel>("InteriorPlayersHouse2FLevel");
	CreateLevel<UInteriorRivalsHouseLevel>("InteriorRivalsHouseLevel");

	// ���� ���� ����
	UEventManager::ChangeLevel("TitleLevel");					 // ���� ������ �� ������ ����
	//UEventManager::ChangeLevel("TutorialLevel");
	//UEventManager::ChangeLevel("InteriorPlayersHouse2FLevel");   // �� ���� ���� ���� ��ġ 
	//UEventManager::ChangeLevel("ExteriorPalletTownLevel");
}