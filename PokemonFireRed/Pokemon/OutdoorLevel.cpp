#include "OutdoorLevel.h"
#include <list>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UOutdoorLevel::UOutdoorLevel()
{
}

UOutdoorLevel::~UOutdoorLevel()
{
}

void UOutdoorLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// ���ҽ� �ε�
	UEngineDirectory CurDir;
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("OutdoorLevel");

	std::list<UEngineFile> AllFiles = CurDir.AllFile();
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}
	
	// �ִϸ��̼� ���ҽ� �ε�
	UEngineResourcesManager::GetInst().CuttingImage("WalkDown.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkUp.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkLeft.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkRight.png", 3, 1);

	// ���� ����
	APlayer* Player = SpawnActor<APlayer>(0);
	AGround* Ground = SpawnActor<AGround>(1);

	Player->SetGround(Ground);
	Ground->SetPlayer(Player);
}
