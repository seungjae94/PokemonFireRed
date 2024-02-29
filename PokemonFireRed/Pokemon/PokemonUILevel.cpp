#include "PokemonUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonCanvas.h"
#include "MapLevel.h"

UPokemonUILevel::UPokemonUILevel()
{
}

UPokemonUILevel::~UPokemonUILevel()
{
}

void UPokemonUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("PokemonUILevel");

	std::list<UEngineFile> Files = CurDir.AllFile({".bmp", ".png"}, true);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Canvas = SpawnActor<APokemonCanvas>();
}

void UPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);

	if (nullptr != MapLevel)
	{
		Canvas->SetPrevMapLevelName(MapLevel->GetName());
		Canvas->Reset();
	}
}
