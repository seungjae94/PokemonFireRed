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

	// ���� ��ǥ ����
	FVector MapInitialPos = { -0.5f, -0.5f };
	MapInitialPos *= Global::F_TILE_SIZE;
	Map->SetActorLocation(MapInitialPos);

	// �̺�Ʈ ���� ����
	AWarp* NewWarp = SpawnEventActor<AWarp>({80, 147});

	// �÷��̾� ��ġ ����
	Player->SetTilePoint({70, 142});
}

