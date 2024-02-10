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

	// 이벤트 트리거 생성
	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownWarp",
		{ 6, 13 }
	);
	AWarp * PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 80, 148 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
}


