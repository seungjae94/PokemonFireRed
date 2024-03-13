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
	Canvas->SetActionBoxImage(RN::PokemonUIActionBox);
}

void UBagItemPokemonUILevel::ProcessItemUseResultMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		UEventManager::FadeChangeLevel(Global::MenuBagUILevel);
	}
}

void UBagItemPokemonUILevel::CancelTargetSelection()
{
	UEventManager::FadeChangeLevel(Global::MenuBagUILevel, false);
	Canvas->SetActionCursor(0);
}

