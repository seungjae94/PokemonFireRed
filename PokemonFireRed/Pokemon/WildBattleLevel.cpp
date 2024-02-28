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

	UEngineResourcesManager::GetInst().CuttingImage(RN::PlayerBattler, 5, 1);

	Canvas = SpawnActor<AWildBattleCanvas>();
}

void UWildBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);
}

void UWildBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	EnemyPokemon = UPlayerData::EnemyWildPokemon;
	Canvas->Init(UPlayerData::GetPokemonInEntry(0), EnemyPokemon);
}

void UWildBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}
