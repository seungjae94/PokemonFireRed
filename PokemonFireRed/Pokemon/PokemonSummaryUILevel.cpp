#include "PokemonSummaryUILevel.h"
#include "PokemonUILevel.h"

UPokemonSummaryUILevel::UPokemonSummaryUILevel()
{
}

UPokemonSummaryUILevel::~UPokemonSummaryUILevel()
{
}

void UPokemonSummaryUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();
	Canvas = SpawnActor<APokemonSummaryCanvas>();
}

void UPokemonSummaryUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
	UPokemonUILevel* PrevLevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);
	Canvas->SetPokemon(PrevLevel->GetTargetPokemon());
	Canvas->RefreshAll();	// UI를 다시 그린다.
	Canvas->Reset();		// 상태를 Info 상태로 되돌린다. 
}
