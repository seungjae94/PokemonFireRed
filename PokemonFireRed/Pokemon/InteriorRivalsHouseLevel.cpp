#include "InteriorRivalsHouseLevel.h"
#include "Warp.h"

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
	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownWarp",
		{ 4, 9 }
	);
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName(Global::ExteriorPalletTownLevel);
	PalletTownWarp->SetTargetPoint({ 79, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
	PalletTownWarp->RegisterPredefinedEvent();
}
