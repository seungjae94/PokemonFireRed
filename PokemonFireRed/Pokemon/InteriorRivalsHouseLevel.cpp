#include "InteriorRivalsHouseLevel.h"
#include "InteriorDoor.h"

UInteriorRivalsHouseLevel::UInteriorRivalsHouseLevel()
{
}

UInteriorRivalsHouseLevel::~UInteriorRivalsHouseLevel()
{
}

void UInteriorRivalsHouseLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PlayerName, { 3, 3 });
	UEventManager::SetDirection(GetName(), Global::PlayerName, FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInit PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 4, 9 });

	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 79, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("RivalsHouseDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();
}
