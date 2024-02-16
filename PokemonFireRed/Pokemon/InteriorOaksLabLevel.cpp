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
	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownDoor",
		{ 6, 13 }
	);
	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownWarpSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 80, 148 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->RegisterPredefinedEvent();
}


