#include "ExteriorPalletTownLevel.h"

#include "Global.h"
#include "ExteriorDoor.h"
#include "DialogueActor.h"
#include "PokemonText.h"

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
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 70, 142 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Down);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting OaksLabDoorSetting = UEventTargetInitialSetting(
		"OaksLabWarp",
		{ 80, 147 }
	);
	AExteriorDoor* OaksLabDoor = SpawnEventTrigger<AExteriorDoor>(OaksLabDoorSetting);
	OaksLabDoor->SetTargetMapName(Global::InteriorOaksLabLevel);
	OaksLabDoor->SetTargetPoint({ 6, 12 });
	OaksLabDoor->SetMoveDirection(FTileVector::Up);
	OaksLabDoor->RegisterPredefinedEvent();

	UEventTargetInitialSetting PlayersHouseDoorSetting = UEventTargetInitialSetting(
		"PlayersHouseWarp",
		{ 70, 141 }
	);
	AExteriorDoor* PlayersHouseDoor = SpawnEventTrigger<AExteriorDoor>(PlayersHouseDoorSetting);
	PlayersHouseDoor->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	PlayersHouseDoor->SetTargetPoint({ 3, 8 });
	PlayersHouseDoor->SetMoveDirection(FTileVector::Up);
	PlayersHouseDoor->RegisterPredefinedEvent();

	UEventTargetInitialSetting RivalsHouseDoorSetting = UEventTargetInitialSetting(
		"RivalsHouseWarp",
		{ 79, 141 }
	);
	AExteriorDoor* RivalsHouseDoor = SpawnEventTrigger<AExteriorDoor>(RivalsHouseDoorSetting);
	RivalsHouseDoor->SetTargetMapName(Global::InteriorRivalsHouseLevel);
	RivalsHouseDoor->SetTargetPoint({ 4, 8 });
	RivalsHouseDoor->SetMoveDirection(FTileVector::Up);
	RivalsHouseDoor->RegisterPredefinedEvent();

	UEventTargetInitialSetting FatManSetting = UEventTargetInitialSetting(
		"FatMan",
		{ 80, 151 },
		FTileVector::Down,
		true,
		true,
		true,
		true
	);
	ADialogueActor* FatMan = SpawnEventTrigger<ADialogueActor>(FatManSetting);
	FatMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	FatMan->SetTextColor(EFontColor::Blue);
	FatMan->RegisterPredefinedEvent();
}
