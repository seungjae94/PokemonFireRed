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
}
