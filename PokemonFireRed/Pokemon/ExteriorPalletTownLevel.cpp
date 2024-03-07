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
	UEventManager::SetPoint(GetName(), Global::Player, { 78, 151 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Down);

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
		{71, 151}, {72, 151}, {73, 151}, {74, 151},
		{71, 152}, {72, 152}, {73, 152}, {74, 152},
		{71, 153}, {72, 153}, {73, 153}, {74, 153},
		{71, 154}, {72, 154}, {73, 154}, {74, 154},
		{71, 155}, {72, 155}, {73, 155}, {74, 155},
		{71, 156}, {72, 156}, {73, 156}, {74, 156},
		{71, 157}, {72, 157}, {73, 157}, {74, 157},
		{71, 158}, {72, 158}, {73, 158}, {74, 158},
	});

	// 디버그용 포켓몬 추가 이벤트
	UPokemon Pokemon0 = UPokemon(EPokedexNo::Rattata, 1);
	UPokemon Pokemon1 = UPokemon(EPokedexNo::Bulbasaur, 30);
	UPokemon Pokemon2 = UPokemon(EPokedexNo::Squirtle, 2);
	UPokemon Pokemon3 = UPokemon(EPokedexNo::Charmander, 7);
	UPokemon Pokemon4 = UPokemon(EPokedexNo::Pidgey, 2);
	UPokemon Pokemon5 = UPokemon(EPokedexNo::Caterpie, 3);
	Pokemon0.SetCurHp(Pokemon0.GetHp() / 6);
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
