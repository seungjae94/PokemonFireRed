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
	UEventManager::SetPoint(GetName(), Global::Player, { 76, 136 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Down);

	// 태초마을 생성
	MakePalletTown();
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

void UExteriorPalletTownLevel::MakePalletTown()
{
	MakePTOaksLabDoor();
	MakePTPlayersHouseDoor();
	MakePTRivalsHouseDoor();
	MakePTTechMan();
	MakePTAnimatedTiles();
}

void UExteriorPalletTownLevel::MakePTOaksLabDoor()
{
	UEventTargetInit OaksLabDoorSetting;
	OaksLabDoorSetting.SetName(EN::OaksLabDoor);
	OaksLabDoorSetting.SetPoint({ 80, 147 });
	OaksLabDoorSetting.SetDirection(FTileVector::Up);
	OaksLabDoorSetting.SetImageName("GreenDoor");
	OaksLabDoorSetting.SetHeight(1);

	AExteriorDoor* OaksLabDoor = SpawnEventTrigger<AExteriorDoor>(OaksLabDoorSetting);
	OaksLabDoor->SetTargetMapName(Global::InteriorOaksLabLevel);
	OaksLabDoor->SetTargetPoint({ 6, 12 });
	OaksLabDoor->SetMoveDirection(FTileVector::Up);
	OaksLabDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTPlayersHouseDoor()
{
	UEventTargetInit PlayersHouseDoorSetting;
	PlayersHouseDoorSetting.SetName(EN::PlayersHouseDoor);
	PlayersHouseDoorSetting.SetPoint({ 70, 141 });
	PlayersHouseDoorSetting.SetDirection(FTileVector::Up);
	PlayersHouseDoorSetting.SetImageName("RedDoor");
	PlayersHouseDoorSetting.SetHeight(1);

	AExteriorDoor* PlayersHouseDoor = SpawnEventTrigger<AExteriorDoor>(PlayersHouseDoorSetting);
	PlayersHouseDoor->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	PlayersHouseDoor->SetTargetPoint({ 3, 8 });
	PlayersHouseDoor->SetMoveDirection(FTileVector::Up);
	PlayersHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTRivalsHouseDoor()
{
	UEventTargetInit RivalsHouseDoorSetting;
	RivalsHouseDoorSetting.SetName(EN::RivalshouseDoor);
	RivalsHouseDoorSetting.SetPoint({ 79, 141 });
	RivalsHouseDoorSetting.SetDirection(FTileVector::Up);
	RivalsHouseDoorSetting.SetImageName("RedDoor");
	RivalsHouseDoorSetting.SetHeight(1);

	AExteriorDoor* RivalsHouseDoor = SpawnEventTrigger<AExteriorDoor>(RivalsHouseDoorSetting);
	RivalsHouseDoor->SetTargetMapName(Global::InteriorRivalsHouseLevel);
	RivalsHouseDoor->SetTargetPoint({ 4, 8 });
	RivalsHouseDoor->SetMoveDirection(FTileVector::Up);
	RivalsHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTTechMan()
{
	UEventTargetInit TechManSetting;
	TechManSetting.SetName(EN::TechMan);
	TechManSetting.SetPoint({ 80, 151 });
	TechManSetting.SetDirection(FTileVector::Down);
	TechManSetting.SetCollidable(true);
	TechManSetting.SetRotatable(true);
	TechManSetting.SetWalkable(true);
	TechManSetting.SetImageNameAuto();

	ADialogueActor* TechMan = SpawnEventTrigger<ADialogueActor>(TechManSetting);
	TechMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	TechMan->SetTextColor(EFontColor::Blue);
	TechMan->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTAnimatedTiles()
{
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
}
