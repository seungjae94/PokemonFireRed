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

	// 맵의 좌표 설정
	FVector MapInitialPos = { -0.5f, -0.5f };
	MapInitialPos *= Global::F_TILE_SIZE;
	Map->SetActorLocation(MapInitialPos);

	// 이벤트 액터 생성
	AWarp* NewWarp = SpawnEventActor<AWarp>({80, 147});

	// 플레이어 위치 지정
	Player->SetTilePoint({70, 142});
}

