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
	Page->RefreshAll();	// UI를 다시 그린다.
	Page->Reset();		// 상태를 Info 상태로 되돌린다. 
}
