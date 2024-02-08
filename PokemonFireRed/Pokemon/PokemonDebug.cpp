#include "PokemonDebug.h"
#include "Global.h"
#include "PokemonMath.h"

PokemonDebug::PokemonDebug() 
{
}

PokemonDebug::~PokemonDebug() 
{
}

void PokemonDebug::ReportPosition(AActor* _Actor, std::string_view _Name)
{
	FVector WorldPos = _Actor->GetActorLocation();
	FVector ScreenPos = WorldPos - _Actor->GetWorld()->GetCameraPos();

	std::string NameStr = _Name.data();
	EngineDebug::OutPutDebugText("<" + NameStr + ">");
	EngineDebug::OutPutDebugText("World: " + WorldPos.ToString());
	EngineDebug::OutPutDebugText("World(Tile): " + FTileVector(WorldPos).ToString());
	EngineDebug::OutPutDebugText("Screen: " + FTileVector(ScreenPos * Global::F_TILE_SIZE).ToString());
}