#include "ExteriorPalletTownLevel.h"

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

	// 월드 좌표 설정
	Map->SetWorldPos({ -70.5f, -142.5f });
}

