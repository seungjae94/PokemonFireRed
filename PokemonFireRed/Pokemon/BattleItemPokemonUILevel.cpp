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
		MsgBoxAssert("UBattleItemPokemonUILevel의 이전 레벨이 UBattleBagUILevel이 아닙니다. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PlayerBattler = BattleBagUILevel->GetPlayerBattler();
	Canvas->SetActionBoxImage(RN::PokemonUIBattleActionBox);
}

void UBattleItemPokemonUILevel::ProcessItemUseResultMessage()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		// 아이템 사용에 성공한 경우
		if (true == ItemUseResult)
		{
			PlayerBattler->SelectItem(UseItem);
			PlayerBattler->SetItemSelectState(EItemSelectState::ItemUsed);
			UEventManager::FadeChangeLevel(Global::BattleLevel);
		}
		// 아이템 사용에 실패한 경우
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


