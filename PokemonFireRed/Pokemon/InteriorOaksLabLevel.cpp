#include "InteriorOaksLabLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Warp.h"

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
		"PalletTownWarp",
		{ 6, 13 }
	);
	AWarp * PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName(Global::ExteriorPalletTownLevel);
	PalletTownWarp->SetTargetPoint({ 80, 148 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
	PalletTownWarp->RegisterPredefinedEvent();
}


