#include "BattleItemPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>

UBattleItemPokemonUILevel::UBattleItemPokemonUILevel() 
{
}

UBattleItemPokemonUILevel::~UBattleItemPokemonUILevel() 
{
}

void UBattleItemPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonUILevel::LevelStart(_PrevLevel);

}

void UBattleItemPokemonUILevel::ProcessItemUseResultMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		UEventManager::FadeChangeLevel(Global::BattleLevel);
	}
}

