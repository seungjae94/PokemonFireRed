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
	Canvas->Reset();
}
