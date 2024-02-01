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

	// ���� ����
	APlayer* Player = SpawnActor<APlayer>(0);
	AGround* Ground = SpawnActor<AGround>(1);

	Player->SetGround(Ground);
	Ground->SetPlayer(Player);
}
