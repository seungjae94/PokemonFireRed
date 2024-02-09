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

	// �̺�Ʈ Ʈ���� ����
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>("PalletTownWarp", { 4, 9 });
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 79, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
}
