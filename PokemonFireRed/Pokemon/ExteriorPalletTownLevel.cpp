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

	// �÷��̾� ���� ��ġ ����
	UEventManager::ChangePoint(GetName(), "Player", { 70, 148 });

	// �̺�Ʈ Ʈ���� ����
	AWarp* OaksLabWarp = SpawnEventTrigger<AWarp>("OaksLabWarp", {80, 147});
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
	OaksLabWarp->SetMoveDirection(FTileVector::Up);
}
