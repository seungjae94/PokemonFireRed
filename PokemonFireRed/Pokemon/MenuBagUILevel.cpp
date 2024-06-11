#include "MenuBagUILevel.h"
#include "MapLevel.h"
#include "PokemonUILevel.h"

UMenuBagUILevel::UMenuBagUILevel() 
{
}

UMenuBagUILevel::~UMenuBagUILevel() 
{
}


void UMenuBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UBagUILevel::LevelStart(_PrevLevel);

	UPokemonUILevel* PokemonUILevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);

	// 포켓몬 UI 레벨에서 되돌아오는 경우 레벨을 초기화하지 않는다.
	if (nullptr != PokemonUILevel)
	{
		return;
	}

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);

	if (nullptr == MapLevel)
	{
		MsgBoxAssert("UMenuBagUILevel의 이전 레벨이 맵 레벨이 아닙니다. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PrevMapName = MapLevel->GetName();
}


void UMenuBagUILevel::SelectTarget()
{
	int RecordCount = UUserData::GetRecordCount(GetCurItemType());
	int TargetIndex = TargetIndexMemory[Page];

	// 취소 버튼을 누른 경우
	if (TargetIndex == RecordCount)
	{
		UEventManager::FadeChangeLevel(PrevMapName);
		return;
	}

	// 맵 레벨에서 몬스터 볼은 사용할 수 없다.
	if (GetCurItemType() == EItemType::PokeBall)
	{
		return;
	}

	State = EBagUIState::ActionSelect;
	Canvas->SetActionItemBoxActive(true);
	Canvas->SetActionItemText(GetTargetItem()->Name + L" is\nselected.");
}

void UMenuBagUILevel::CancelTargetSelection()
{
	UEventManager::FadeChangeLevel(PrevMapName);
}

void UMenuBagUILevel::SelectAction()
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

	// 포켓몬이 없는 경우 액션 선택을 막아둔다.
	if (UUserData::GetPokemonEntrySize() == 0)
	{
		State = EBagUIState::TargetSelect;
	}
	// 포켓몬이 있는 경우 포켓몬 페이지로 간다.
	else
	{
		State = EBagUIState::TargetSelect;
		UEventManager::FadeChangeLevel(Global::BagItemPokemonUILevel);
	}

	// 액션을 마치고 나면 액션창은 꺼둔다.
	Canvas->SetActionCursor(0);
	Canvas->SetActionItemBoxActive(false);
}

