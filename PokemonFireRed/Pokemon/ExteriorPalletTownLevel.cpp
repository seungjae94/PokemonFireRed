#include "ExteriorPalletTownLevel.h"

#include "Global.h"
#include "Warp.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel() 
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel() 
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정 (디버깅 용도)
	UEventManager::ChangePoint(GetName(), "Player", { 70, 142 });
	UEventManager::ChangeDirection(GetName(), "Player", FTileVector::Down);

	// 이벤트 트리거 생성
	AWarp* OaksLabWarp = SpawnEventTrigger<AWarp>("OaksLabWarp", {80, 147});
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
	OaksLabWarp->SetMoveDirection(FTileVector::Up);

	AWarp* PlayersHouseWarp = SpawnEventTrigger<AWarp>("PlayersHouseWarp", { 70, 141 });
	PlayersHouseWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	PlayersHouseWarp->SetTargetPoint({ 3, 8 });
	PlayersHouseWarp->SetMoveDirection(FTileVector::Up);

	AWarp* RivalsHouseWarp = SpawnEventTrigger<AWarp>("RivalsHouseWarp", { 79, 141 });
	RivalsHouseWarp->SetTargetLevelName("InteriorRivalsHouseLevel");
	RivalsHouseWarp->SetTargetPoint({ 4, 8 });
	RivalsHouseWarp->SetMoveDirection(FTileVector::Up);
}
