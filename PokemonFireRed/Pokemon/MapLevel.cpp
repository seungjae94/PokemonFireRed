#include "MapLevel.h"

UMapLevel::UMapLevel() 
{
}

UMapLevel::~UMapLevel() 
{
}

void UMapLevel::BeginPlay()
{
	static bool IsCalled = false;

	if (true == IsCalled)
	{
		return;
	}
	IsCalled = true;

	// 플레이어 리소스 로드
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({".png", ".bmp"}, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// 플레이어 애니메이션 리소스 로드
	UEngineResourcesManager::GetInst().CuttingImage("WalkDown.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkUp.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkLeft.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkRight.png", 3, 1);

	// 액터 생성
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	Player->SetMap(Map);
	Map->SetPlayer(Player);
}

