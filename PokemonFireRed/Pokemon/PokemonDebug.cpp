#include "PokemonDebug.h"
#include "Global.h"
#include "PokemonMath.h"
#include "EventTarget.h"

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
	UEngineDebug::OutPutDebugText("<" + NameStr + ">");
	UEngineDebug::OutPutDebugText("Name: " + _Actor->GetName());
	UEngineDebug::OutPutDebugText("World: " + WorldPos.ToString());
	UEngineDebug::OutPutDebugText("World(Tile): " + FTileVector(WorldPos).ToString());
	UEngineDebug::OutPutDebugText("Screen: " + FTileVector(ScreenPos * Global::FloatTileSize).ToString());
}

void PokemonDebug::ReportPosition(AEventTarget* _Actor, std::string_view _Name)
{
	FVector WorldPos = _Actor->GetActorLocation();
	FVector ScreenPos = WorldPos - _Actor->GetWorld()->GetCameraPos();

	std::string NameStr = _Name.data();
	UEngineDebug::OutPutDebugText("<" + NameStr + ">");
	UEngineDebug::OutPutDebugText("Name: " + _Actor->GetName());
	UEngineDebug::OutPutDebugText("World: " + WorldPos.ToString());
	UEngineDebug::OutPutDebugText("World(Tile): " + FTileVector(WorldPos).ToString());
	UEngineDebug::OutPutDebugText("Point: " + _Actor->Point.ToString());
	UEngineDebug::OutPutDebugText("Screen: " + FTileVector(ScreenPos * Global::FloatTileSize).ToString());
}

void PokemonDebug::ReportFrameRate(float _DeltaTime)
{
	float FPS = 1.0f / _DeltaTime;
	UEngineDebug::OutPutDebugText(std::to_string(FPS));
}