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

	CurDir.Move("InteriorOaksLabLevel");

	std::list<UEngineFile> Files = CurDir.AllFile();

	for (UEngineFile& File : Files)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	Map->SetBackgroundImage("InteriorOaksLabBackground.png");
	Map->SetForegroundImage("InteriorOaksLabForeground.png");
	Map->SetCollisionImage("InteriorOaksLabCollision.png");
	Map->SetCollisionRendererActive(false);
	
	// 이벤트 액터 생성
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>("PalletTownWarp", { 6, 13 });
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 80, 148 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
}


