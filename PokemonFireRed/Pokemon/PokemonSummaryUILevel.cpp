#include "PokemonSummaryUILevel.h"

UPokemonSummaryUILevel::UPokemonSummaryUILevel()
{
}

UPokemonSummaryUILevel::~UPokemonSummaryUILevel()
{
}

void UPokemonSummaryUILevel::BeginPlay()
{
	Page = SpawnActor<APokemonSummaryPage>();
}

void UPokemonSummaryUILevel::Tick(float _DeltaTime)
{
}
