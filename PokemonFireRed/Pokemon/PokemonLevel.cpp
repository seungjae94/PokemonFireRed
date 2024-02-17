#include "PokemonLevel.h"
#include "EventManager.h"
#include "BlackScreen.h"

UPokemonLevel::UPokemonLevel()
{
}

UPokemonLevel::~UPokemonLevel()
{
}

void UPokemonLevel::BeginPlay()
{
	// ����â ����
	ABlackScreen* BlackScreen = SpawnUIElement<ABlackScreen>("BlackScreen");
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

