#include "OutdoorLevel.h"
#include <list>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Player.h"
#include "Ground.h"

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

	// ���� ����
	APlayer* Player = SpawnActor<APlayer>();
	AGround* Ground = SpawnActor<AGround>();
	Player->SetGround(Ground);
}
