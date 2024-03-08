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
	// ���� �⺻ ����
	MainWindow.SetWindowScale({ Global::ScreenX, Global::ScreenY });
	MainWindow.SetClearColor(Color8Bit::BlackA);

	// UI ���ҽ� �ε�
	LoadUIResources();

	// ���ϸ� ���ҽ� �ε�
	LoadPokemonResources();

	// ���� ���ҽ� �ε�
	LoadSounds();

	// ����׿� ���ϸ� �߰� - �ݵ�� ���� ���� ������ ����
	UPokemon Pokemon0 = UPokemon(EPokedexNo::Bulbasaur, 10);
	UPokemon Pokemon1 = UPokemon(EPokedexNo::Rattata, 1);
	UPokemon Pokemon2 = UPokemon(EPokedexNo::Squirtle, 2);
	UPokemon Pokemon3 = UPokemon(EPokedexNo::Charmander, 7);
	UPokemon Pokemon4 = UPokemon(EPokedexNo::Pidgey, 2);
	UPokemon Pokemon5 = UPokemon(EPokedexNo::Caterpie, 3);
	Pokemon1.SetCurHp(Pokemon0.GetHp() / 6);
	Pokemon3.SetCurHp(Pokemon3.GetHp() / 3);
	Pokemon0.AddAccExp(0);
	Pokemon1.AddAccExp(15);
	Pokemon2.AddAccExp(5);
	Pokemon3.AddAccExp(3);
	Pokemon4.AddAccExp(1);
	UPlayerData::AddPokemonToEntry(Pokemon0);
	UPlayerData::AddPokemonToEntry(Pokemon1);
	UPlayerData::AddPokemonToEntry(Pokemon2);
	UPlayerData::AddPokemonToEntry(Pokemon3);
	UPlayerData::AddPokemonToEntry(Pokemon4);
	UPlayerData::AddPokemonToEntry(Pokemon5);
	UPlayerData::Achieve(EAchievement::GetFirstPokemon);

	// ���� ����
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

	// ���� ���� ����
	UEventManager::SetLevel(Global::InteriorOaksLabLevel);
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

	// ���� �ε�
	std::list<UEngineFile> AllSounds = CurDir.AllFile({ ".mp3", ".wav" });
	for (UEngineFile& Sound : AllSounds)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	CurDir.MoveParent();
}
