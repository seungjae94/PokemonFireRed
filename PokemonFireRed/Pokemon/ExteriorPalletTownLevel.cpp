#include "ExteriorPalletTownLevel.h"
#include "Global.h"
#include "ExteriorDoor.h"
#include "DialogueActor.h"
#include "Text.h"
#include "PlayerData.h"
#include "Pokemon.h"
#include "SoundManager.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel()
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel()
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정 (디버깅 용도)
	UEventManager::SetPoint(GetName(), Global::PlayerName, { 70, 142 });
	UEventManager::SetDirection(GetName(), Global::PlayerName, FTileVector::Down);

	// 이벤트 트리거 생성
	UEventTargetInit OaksLabDoorSetting;
	OaksLabDoorSetting.SetName("OaksLabDoor");
	OaksLabDoorSetting.SetPoint({ 80, 147 });
	OaksLabDoorSetting.SetDirection(FTileVector::Up);
	OaksLabDoorSetting.SetImageName("GreenDoor");
	OaksLabDoorSetting.SetHeight(1);

	AExteriorDoor* OaksLabDoor = SpawnEventTrigger<AExteriorDoor>(OaksLabDoorSetting);
	OaksLabDoor->SetTargetMapName(Global::InteriorOaksLabLevel);
	OaksLabDoor->SetTargetPoint({ 6, 12 });
	OaksLabDoor->SetMoveDirection(FTileVector::Up);
	OaksLabDoor->RegisterPredefinedEvent();


	UEventTargetInit PlayersHouseDoorSetting;
	PlayersHouseDoorSetting.SetName("PlayersHouseDoor");
	PlayersHouseDoorSetting.SetPoint({ 70, 141 });
	PlayersHouseDoorSetting.SetDirection(FTileVector::Up);
	PlayersHouseDoorSetting.SetImageName("RedDoor");
	PlayersHouseDoorSetting.SetHeight(1);

	AExteriorDoor* PlayersHouseDoor = SpawnEventTrigger<AExteriorDoor>(PlayersHouseDoorSetting);
	PlayersHouseDoor->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	PlayersHouseDoor->SetTargetPoint({ 3, 8 });
	PlayersHouseDoor->SetMoveDirection(FTileVector::Up);
	PlayersHouseDoor->RegisterPredefinedEvent();


	UEventTargetInit RivalsHouseDoorSetting;
	RivalsHouseDoorSetting.SetName("RivalsHouseDoor");
	RivalsHouseDoorSetting.SetPoint({ 79, 141 });
	RivalsHouseDoorSetting.SetDirection(FTileVector::Up);
	RivalsHouseDoorSetting.SetImageName("RedDoor");
	RivalsHouseDoorSetting.SetHeight(1);

	AExteriorDoor* RivalsHouseDoor = SpawnEventTrigger<AExteriorDoor>(RivalsHouseDoorSetting);
	RivalsHouseDoor->SetTargetMapName(Global::InteriorRivalsHouseLevel);
	RivalsHouseDoor->SetTargetPoint({ 4, 8 });
	RivalsHouseDoor->SetMoveDirection(FTileVector::Up);
	RivalsHouseDoor->RegisterPredefinedEvent();


	UEventTargetInit FatManSetting;
	FatManSetting.SetName("FatMan");
	FatManSetting.SetPoint({ 80, 151 });
	FatManSetting.SetDirection(FTileVector::Down);
	FatManSetting.SetCollidable(true);
	FatManSetting.SetRotatable(true);
	FatManSetting.SetWalkable(true);
	FatManSetting.SetImageNameAuto();

	ADialogueActor* FatMan = SpawnEventTrigger<ADialogueActor>(FatManSetting);
	FatMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	FatMan->SetTextColor(EFontColor::Blue);
	FatMan->RegisterPredefinedEvent();

	DrawFlowers({
		{69, 146}, {69, 147},
		{70, 146}, {70, 147},
		{71, 146}, {71, 147},
		{72, 146}, {72, 147},
	});

	DrawSeas({
		{71, 151, 0}, {72, 151, 0}, {73, 151, 0}, {74, 151, 0},
		{71, 152, 0}, {72, 152, 0}, {73, 152, 0}, {74, 152, 0},
		{71, 153, 0}, {72, 153, 0}, {73, 153, 0}, {74, 153, 0},
		{71, 154, 0}, {72, 154, 0}, {73, 154, 0}, {74, 154, 0},
		{71, 155, 0}, {72, 155, 0}, {73, 155, 0}, {74, 155, 0},
		{71, 156, 0}, {72, 156, 0}, {73, 156, 0}, {74, 156, 0},
		{71, 157, 0}, {72, 157, 0}, {73, 157, 0}, {74, 157, 0},
		{71, 158, 0}, {72, 158, 0}, {73, 158, 0}, {74, 158, 0},
	});

	// 디버그용 포켓몬 추가 이벤트
	UPokemon Pokemon0 = UPokemon(EPokedexNo::Bulbasaur);
	UPokemon Pokemon1 = UPokemon(EPokedexNo::Charmander);
	UPokemon Pokemon2 = UPokemon(EPokedexNo::Squirtle);
	Pokemon0.SetCurHp(Pokemon0.GetHp() / 2);
	Pokemon1.SetCurHp(Pokemon1.GetHp() / 6);
	Pokemon2.HealAll();
	Pokemon0.GainExp(20);
	Pokemon1.GainExp(5);
	UPlayerData::AddPokemonToEntry(Pokemon0);
	UPlayerData::AddPokemonToEntry(Pokemon1);
	UPlayerData::AddPokemonToEntry(Pokemon2);
	UPlayerData::Achieve(EAchievement::GetFirstPokemon);
}

void UExteriorPalletTownLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	if (nullptr == _PrevLevel)
	{
		// 디버그용
		USoundManager::PlayBgm(RN::BgmPalletTown);
	}
}
