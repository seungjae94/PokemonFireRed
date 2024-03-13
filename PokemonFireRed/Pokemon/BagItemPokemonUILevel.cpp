#include "BagItemPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "BagUILevel.h"

UBagItemPokemonUILevel::UBagItemPokemonUILevel() 
{
}

UBagItemPokemonUILevel::~UBagItemPokemonUILevel() 
{
}

void UBagItemPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UItemPokemonUILevel::LevelStart(_PrevLevel);

}

void UBagItemPokemonUILevel::ProcessItemUseResultMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		UEventManager::FadeChangeLevel(Global::BagUILevel);
	}
}

