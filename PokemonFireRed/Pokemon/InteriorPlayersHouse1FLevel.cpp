#include "InteriorPlayersHouse1FLevel.h"
#include "Warp.h"

UInteriorPlayersHouse1FLevel::UInteriorPlayersHouse1FLevel()
{
}

UInteriorPlayersHouse1FLevel::~UInteriorPlayersHouse1FLevel()
{
}

void UInteriorPlayersHouse1FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 이벤트 트리거 생성
	AWarp* StairTo2FWarp = SpawnEventTrigger<AWarp>("StairTo2FWarp", { 10, 2 });
	StairTo2FWarp->SetTargetLevelName("InteriorPlayersHouse2FLevel");
	StairTo2FWarp->SetTargetPoint({ 9, 2 });
	StairTo2FWarp->SetMoveDirection(FTileVector::Right);

	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>("PalletTownWarp", { 3, 9 });
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 70, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
}
