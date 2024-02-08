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

	CurDir.Move("ExteriorPalletTownLevel");

	std::list<UEngineFile> AllFiles = CurDir.AllFile();
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	Map->SetBackgroundImage("ExteriorPalletTownBackground.png");
	Map->SetForegroundImage("ExteriorPalletTownForeground.png");
	Map->SetCollisionImage("ExteriorPalletTownCollision.png");
	Map->SetCollisionRendererActive(false);

	// 이벤트 액터 생성
	AWarp* OaksLabWarp = SpawnEventActor<AWarp>({80, 147});
	OaksLabWarp->SetPlayer(Player);
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
}
