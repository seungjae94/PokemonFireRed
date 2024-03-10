#include "BagUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UBagUILevel::UBagUILevel()
{
}

UBagUILevel::~UBagUILevel()
{
}

void UBagUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("BagUILevel");

	std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Canvas = SpawnActor<ABagCanvas>();
}

void UBagUILevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);
}

void UBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	PrevLevelName = _PrevLevel->GetName();

	Canvas->Init();
}

void UBagUILevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);

}
