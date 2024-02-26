#include "TrainerCardUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UTrainerCardUILevel::UTrainerCardUILevel()
{
}

UTrainerCardUILevel::~UTrainerCardUILevel()
{
}

void UTrainerCardUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("TrainerCardUILevel");

	std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Page = SpawnActor<ATrainerCardPage>();
}

void UTrainerCardUILevel::LevelStart(ULevel* _PrevLevel)
{
	std::string PrevMapLevelName = _PrevLevel->GetName();
	Page->SetPrevMapLevelName(PrevMapLevelName);
	Page->RefreshAll();
}
