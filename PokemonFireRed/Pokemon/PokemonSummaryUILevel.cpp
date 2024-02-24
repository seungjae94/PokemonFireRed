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
	Page = SpawnActor<APokemonSummaryPage>();
}

void UPokemonSummaryUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
	UPokemonUILevel* PrevLevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);
	Page->SetPokemon(PrevLevel->GetTargetPokemon());
	Page->RefreshAll();
}
