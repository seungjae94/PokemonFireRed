#include "WildBattleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
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



}

void UWildBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);
}

void UWildBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
}

void UWildBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}
