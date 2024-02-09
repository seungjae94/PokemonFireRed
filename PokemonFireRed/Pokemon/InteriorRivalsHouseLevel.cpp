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

	// 이벤트 트리거 생성
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>("PalletTownWarp", { 4, 9 });
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 79, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
}
