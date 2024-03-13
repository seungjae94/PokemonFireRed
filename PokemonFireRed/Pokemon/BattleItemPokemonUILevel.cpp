#include "BattleItemPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleBagUILevel.h"

UBattleItemPokemonUILevel::UBattleItemPokemonUILevel() 
{
}

UBattleItemPokemonUILevel::~UBattleItemPokemonUILevel() 
{
}

void UBattleItemPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UItemPokemonUILevel::LevelStart(_PrevLevel);

	UBattleBagUILevel* BattleBagUILevel = dynamic_cast<UBattleBagUILevel*>(_PrevLevel);

	if (nullptr == BattleBagUILevel)
	{
		MsgBoxAssert("UBattleItemPokemonUILevel�� ���� ������ UBattleBagUILevel�� �ƴմϴ�. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PlayerBattler = BattleBagUILevel->GetPlayerBattler();
	Canvas->SetActionBoxImage(RN::PokemonUIBattleActionBox);
}

void UBattleItemPokemonUILevel::ProcessItemUseResultMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		// ������ ��뿡 ������ ���
		if (true == ItemUseResult)
		{
			PlayerBattler->SelectItem(UseItem);
			PlayerBattler->SetItemSelectState(EItemSelectState::ItemUsed);
			UEventManager::FadeChangeLevel(Global::BattleLevel);
		}
		// ������ ��뿡 ������ ���
		else
		{
			UEventManager::FadeChangeLevel(Global::BattleBagUILevel);
		}
	}
}

void UBattleItemPokemonUILevel::CancelTargetSelection()
{
	UEventManager::FadeChangeLevel(Global::BattleBagUILevel, false);
	Canvas->SetActionCursor(0);
}


