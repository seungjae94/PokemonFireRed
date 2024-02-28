#include "WildBattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PlayerData.h"
#include "PokemonUtil.h"

UWildBattleLevel::UWildBattleLevel() 
{
}

UWildBattleLevel::~UWildBattleLevel() 
{
}

void UWildBattleLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();
	UPokemonUtil::LoadAllResourcesIn(RN::BattleLevel);

	Canvas = SpawnActor<AWildBattleCanvas>();
}

void UWildBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);
}

void UWildBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	Pokemon = UPlayerData::EnemyWildPokemon;
	Canvas->BattleStart(Pokemon);
}

void UWildBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}
