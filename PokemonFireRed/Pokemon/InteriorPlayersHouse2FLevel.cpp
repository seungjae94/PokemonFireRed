#include "InteriorPlayersHouse2FLevel.h"
#include "Warp.h"

UInteriorPlayersHouse2FLevel::UInteriorPlayersHouse2FLevel()
{
}

UInteriorPlayersHouse2FLevel::~UInteriorPlayersHouse2FLevel()
{
}

void UInteriorPlayersHouse2FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::ChangePoint(GetName(), "Player", { 5, 6 });
	UEventManager::ChangeDirection(GetName(), "Player", FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo1FWarpSetting = UEventTargetInitialSetting(
		"StairTo1FWarp",
		{ 8, 2 }
	);
	AWarp* StairTo1FWarp = SpawnEventTrigger<AWarp>(StairTo1FWarpSetting);
	StairTo1FWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	StairTo1FWarp->SetTargetPoint({9, 2});
	StairTo1FWarp->SetMoveDirection(FTileVector::Left);
}
