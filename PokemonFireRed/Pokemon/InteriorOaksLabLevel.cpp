#include "InteriorOaksLabLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "InteriorDoor.h"

UInteriorOaksLabLevel::UInteriorOaksLabLevel()
{
}

UInteriorOaksLabLevel::~UInteriorOaksLabLevel()
{
}

void UInteriorOaksLabLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 6, 6 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Up);


	// �̺�Ʈ Ʈ���� ����
	UEventTargetInit PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 6, 13 });
	
	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 80, 148 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("OaksLabDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();
}


