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

	// �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::PlayerName, { 3, 3 });
	UEventManager::SetDirection(GetName(), Global::PlayerName, FTileVector::Up);

	// �̺�Ʈ Ʈ���� ����
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
