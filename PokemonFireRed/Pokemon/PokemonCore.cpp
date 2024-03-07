#include "PokemonCore.h"
#include "TitleLevel.h"
#include "TutorialLevel.h"
#include "ExteriorPalletTownLevel.h"
#include "InteriorOaksLabLevel.h"
#include "InteriorPlayersHouse1FLevel.h"
#include "InteriorPlayersHouse2FLevel.h"
#include "InteriorRivalsHouseLevel.h"
#include "PokemonUILevel.h"
#include "PokemonSummaryUILevel.h"
#include "TrainerCardUILevel.h"
#include "BattleLevel.h"
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
	MainWindow.SetWindowScale({ Global::ScreenX, Global::ScreenY });
	MainWindow.SetClearColor(Color8Bit::BlackA);

	// UI 리소스 로딩
	LoadUIResources();

	// 포켓몬 리소스 로딩
	LoadPokemonResources();

	// 사운드 리소스 로딩
	LoadSounds();

	// 레벨 생성
	CreateLevel<UTitleLevel>(Global::TitleLevel);
	CreateLevel<UTutorialLevel>(Global::TutorialLevel);
	CreateLevel<UExteriorPalletTownLevel>(Global::ExteriorPalletTownLevel);
	CreateLevel<UInteriorOaksLabLevel>(Global::InteriorOaksLabLevel);
	CreateLevel<UInteriorPlayersHouse1FLevel>(Global::InteriorPlayersHouse1FLevel);
	CreateLevel<UInteriorPlayersHouse2FLevel>(Global::InteriorPlayersHouse2FLevel);
	CreateLevel<UInteriorRivalsHouseLevel>(Global::InteriorRivalsHouseLevel);
	CreateLevel<UPokemonUILevel>(Global::PokemonUILevel);
	CreateLevel<UPokemonSummaryUILevel>(Global::PokemonSummaryUILevel);
	CreateLevel<UTrainerCardUILevel>(Global::TrainerCardUILevel);
	CreateLevel<UBattleLevel>(Global::BattleLevel);

	// 시작 레벨 설정
	UEventManager::SetLevel(Global::InteriorPlayersHouse1FLevel);
}

void UPokemonCore::Tick(float _DeltaTime)
{
	if (UEngineInput::IsDown('F') && _DeltaTime > 0.0f)
	{
		UEngineDebug::OutPutDebugText(std::to_string(1 / _DeltaTime));
	}
}

void UPokemonCore::LoadUIResources()
{
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("UI");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	UEngineResourcesManager::GetInst().CuttingImage(RN::NextMsgArrow, 4, 1);

	CurDir.MoveParent();
}

void UPokemonCore::LoadPokemonResources()
{
	CurDir.Move("Pokemons");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, true);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	UEngineResourcesManager::GetInst().CuttingImage("PokemonBack.png", 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage("PokemonFront.png", 2, 154);
	UEngineResourcesManager::GetInst().CuttingImage("PokemonMini.png", 2, 154);

	CurDir.MoveParent();
}

void UPokemonCore::LoadSounds()
{
	CurDir.Move("Bgm");

	// 사운드 로드
	std::list<UEngineFile> AllSounds = CurDir.AllFile({ ".mp3", ".wav" });
	for (UEngineFile& Sound : AllSounds)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	CurDir.MoveParent();
}
