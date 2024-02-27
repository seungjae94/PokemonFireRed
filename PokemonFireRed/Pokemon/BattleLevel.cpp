#include "BattleLevel.h"

UBattleLevel::UBattleLevel() 
{
}

UBattleLevel::~UBattleLevel() 
{
}

void UBattleLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();
}

void UBattleLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);
}

void UBattleLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
}

void UBattleLevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}
