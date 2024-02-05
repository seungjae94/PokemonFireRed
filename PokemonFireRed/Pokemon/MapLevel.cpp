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

	// �÷��̾� ���ҽ� �ε�
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({".png", ".bmp"}, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// �÷��̾� �ִϸ��̼� ���ҽ� �ε�
	UEngineResourcesManager::GetInst().CuttingImage("WalkDown.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkUp.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkLeft.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkRight.png", 4, 1);

	// ���� ����
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	Player->SetMap(Map);
	Map->SetPlayer(Player);
}

