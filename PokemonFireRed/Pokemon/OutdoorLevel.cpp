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

	// 리소스 로드
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

	// 액터 생성
	APlayer* Player = SpawnActor<APlayer>();
	AGround* Ground = SpawnActor<AGround>();
	Player->SetGround(Ground);
}
