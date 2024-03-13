#include "BattleBagUILevel.h"
#include "Battler.h"
#include "BattleLevel.h"
#include "PokemonUILevel.h"

UBattleBagUILevel::UBattleBagUILevel() 
{
}

UBattleBagUILevel::~UBattleBagUILevel() 
{
}

UBattler* UBattleBagUILevel::GetPlayerBattler()
{
	return PlayerBattler;
}

void UBattleBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UBagUILevel::LevelStart(_PrevLevel);

	UPokemonUILevel* PokemonUILevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);

	// 포켓몬 UI 레벨에서 되돌아오는 경우 레벨을 초기화하지 않는다.
	if (nullptr != PokemonUILevel)
	{
		return;
	}


	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("UBattleBagUILevel의 이전 레벨이 배틀 레벨이 아닙니다. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PlayerBattler = BattleLevel->GetPlayerBattler();
}

void UBattleBagUILevel::SelectTarget()
{
	int RecordCount = UPlayerData::GetRecordCount(GetCurItemType());
	int TargetIndex = TargetIndexMemory[Page];

	// 취소 버튼을 누른 경우
	if (TargetIndex == RecordCount)
	{
		// 아이템 사용을 취소했다고 마킹을 해줘야 한다.
		PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
		UEventManager::FadeChangeLevel(Global::BattleLevel);
		return;
	}

	State = EBagUIState::ActionSelect;
	Canvas->SetActionItemBoxActive(true);
	Canvas->SetActionItemText(GetTargetItem()->Name + L" is\nselected.");
}

void UBattleBagUILevel::CancelTargetSelection()
{
	// 아이템 사용을 취소했다고 마킹을 해줘야 한다.
	PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
	UEventManager::FadeChangeLevel(Global::BattleLevel);
}

void UBattleBagUILevel::SelectAction()
{
	int Cursor = Canvas->GetActionCursor();

	// 취소 액션을 선택한 경우 액션창을 끄고 다시 아이템을 선택한다.
	if (Cursor == 1)
	{
		State = EBagUIState::TargetSelect;
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		return;
	}

	// 몬스터 볼을 선택한 경우 아이템 사용 결과를 플레이어 배틀러에 넣어주고 배틀 레벨로 돌아간다.
	if (GetCurItemType() == EItemType::PokeBall)
	{
		State = EBagUIState::TargetSelect;
		PlayerBattler->SelectItem(GetTargetItem());
		PlayerBattler->SetItemSelectState(EItemSelectState::BallSelected);
		UEventManager::FadeChangeLevel(Global::BattleLevel);
	}
	// 소비 아이템을 선택한 경우 아이템 사용 결과를 플레이어 배틀러에 넣어주는 건 UBattleItemPokemonUILevel이 담당한다.
	else if (GetCurItemType() == EItemType::UseItem)
	{
		State = EBagUIState::TargetSelect;
		UEventManager::FadeChangeLevel(Global::BattleItemPokemonUILevel);
	}

	// 액션을 마치고 나면 액션창은 꺼둔다.
	Canvas->SetActionCursor(0);
	Canvas->SetActionItemBoxActive(false);
}
