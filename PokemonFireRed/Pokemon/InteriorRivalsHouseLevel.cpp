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
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 3, 3 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Up);

	// �̺�Ʈ Ʈ���� ����
	UEventTargetInitialSetting PalletTownDoorSetting = UEventTargetInitialSetting(
		"PalletTownDoor",
		{ 4, 9 }
	);
	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 79, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->RegisterPredefinedEvent();
}
