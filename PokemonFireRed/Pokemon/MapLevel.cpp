#include "MapLevel.h"
#include <EnginePlatform/EngineInput.h>

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
	UEngineResourcesManager::GetInst().CuttingImage("WalkDown.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkUp.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkLeft.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkRight.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("JumpDown.png", 53, 1);

	// 액터 생성
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	Player->SetMap(Map);
	Map->SetPlayer(Player);
}

void UMapLevel::Tick(float _DeltaTime)
{
	if (EngineInput::IsDown(VK_F1))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}
}

