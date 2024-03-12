#include "BagUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "MapLevel.h"
#include "BattleLevel.h"
#include "PokemonUILevel.h"
#include "Battler.h"

UBagUILevel::UBagUILevel()
{
}

UBagUILevel::~UBagUILevel()
{
}

bool UBagUILevel::IsBattleMode() const
{
	return BattleMode;
}

void UBagUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("BagUILevel");

	std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Canvas = SpawnActor<ABagCanvas>();
}

void UBagUILevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	switch (State)
	{
	case EState::None:
		break;
	case EState::TargetSelect:
		ProcessTargetSelect();
		break;
	case EState::ActionSelect:
		ProcessActionSelect();
		break;
	case EState::BattleModeItemUsageCheck:
		ProcessBattleModeItemUsageCheck();
		break;
	case EState::End:
		break;
	default:
		break;
	}

}

void UBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);
	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);
	UPokemonUILevel* PokemonUILevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);

	// 포켓몬 UI 레벨에서 되돌아오는 경우 레벨을 초기화하지 않는다.
	if (nullptr != PokemonUILevel)
	{
		return;
	}

	BattleMode = false;
	if (nullptr != MapLevel)
	{
		PrevLevelName = _PrevLevel->GetName();
		BattleMode = false;
	}
	else if (nullptr != BattleLevel)
	{
		PrevLevelName = _PrevLevel->GetName();
		PlayerBattler = BattleLevel->GetPlayerBattler();
		BattleMode = true;
	}
	RefreshPage();

	State = EState::TargetSelect;
	Canvas->SetActionItemBoxActive(false);
}

void UBagUILevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);
}

void UBagUILevel::ProcessTargetSelect()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		int RecordCount = UPlayerData::GetRecordCount(PageToItemType(Page));
		int TargetIndex = TargetIndexMemory[Page];

		// 취소 버튼을 누른 경우
		if (TargetIndex == RecordCount)
		{
			// 배틀 모드에서 되돌아가는 경우 아이템 사용을 취소했다고 마킹을 해줘야 한다.
			if (true == BattleMode)
			{
				PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
			}
			UEventManager::FadeChangeLevel(PrevLevelName);
			return;
		}

		// 배틀 모드가 아닐 때, 즉 맵 레벨에서 몬스터 볼은 사용할 수 없다.
		if (false == BattleMode && Page == 2)
		{
			return;
		}

		State = EState::ActionSelect;
		Canvas->SetActionItemBoxActive(true);
		Canvas->SetActionItemText(GetTargetItem()->Name + L" is\nselected.");
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		// 배틀 모드에서 되돌아가는 경우 아이템 사용을 취소했다고 마킹을 해줘야 한다.
		if (true == BattleMode)
		{
			PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
		}

		UEventManager::FadeChangeLevel(PrevLevelName);
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		ScrollUp();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		ScrollDown();
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		if (Page + 1 <= 2)
		{
			++Page;
			RefreshPage();
			return;
		}
	}

	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		if (Page - 1 >= 0)
		{
			--Page;
			RefreshPage();
			return;
		}
	}
}

void UBagUILevel::ProcessActionSelect()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		int Cursor = Canvas->GetActionCursor();

		// 취소 액션을 선택한 경우 액션창을 끄고 다시 아이템을 선택한다.
		if (Cursor == 1)
		{
			State = EState::TargetSelect;
			Canvas->SetActionCursor(0);
			Canvas->SetActionItemBoxActive(false);
			return;
		}
		
		// 배틀 모드에서 몬스터 볼을 선택한 경우
		if (true == BattleMode && PageToItemType(Page) == EItemType::PokeBall)
		{
			State = EState::TargetSelect;
			PlayerBattler->SetItemSelectState(EItemSelectState::BallSelected);
			UEventManager::FadeChangeLevel(PrevLevelName);
		}
		// 배틀 모드에서 소비 아이템을 선택한 경우
		else if (true == BattleMode && PageToItemType(Page) == EItemType::UseItem)
		{
			State = EState::BattleModeItemUsageCheck;
			ItemUsage = EItemUsage::None;
			UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		}
		// 배틀 모드가 아니고 소비 아이템을 선택했지만 포켓몬이 없는 경우
		else if (UPlayerData::GetPokemonEntrySize() == 0)
		{
			State = EState::TargetSelect;
		}
		// 배틀 모드가 아니고 소비 아이템을 선택했으며 포켓몬이 있는 경우
		else
		{
			State = EState::TargetSelect;
			UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		}

		// 액션을 마치고 나면 액션창은 꺼둔다.
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		return;
	}

	// 액션창을 끄고 다시 아이템을 선택한다.
	if (true == UEngineInput::IsDown('X'))
	{
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		State = EState::TargetSelect;
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Canvas->DecActionCursor();
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->IncActionCursor();
		return;
	}
}

void UBagUILevel::ProcessBattleModeItemUsageCheck()
{
	// 소비 아이템을 사용했다.
	if (EItemUsage::Used == ItemUsage)
	{
		State = EState::TargetSelect;
		PlayerBattler->SetItemSelectState(EItemSelectState::ItemUsed);
		UEventManager::FadeChangeLevel(PrevLevelName);
	}
	// 소비 아이템을 사용하지 않았다. 다시 아이템을 선택한다.
	else if (EItemUsage::NotUsed == ItemUsage)
	{
		State = EState::TargetSelect;
	}
}

void UBagUILevel::ScrollUp()
{
	int& StartIndex = StartIndexMemory[Page];
	int& TargetIndex = TargetIndexMemory[Page];
	int RecordCount = UPlayerData::GetRecordCount(PageToItemType(Page));

	// 0. 커서를 더 올릴 수 없는 경우
	if (TargetIndex == 0)
	{
		return;
	}

	// 1. RecordCount < 6인 경우 (리스트 스크롤이 발생할 가능성이 아예 없는 경우)
	if (RecordCount < 6)
	{
		--TargetIndex;
	}
	// 2. RecordCount >= 6인 경우 (리스트 스크롤이 발생할 가능성은 있는 경우)
	// 2-1. 리스트가 위로 쭉 땡겨져 있고 리스트 스크롤이 발생하지 않는 경우
	else if (TargetIndex <= 2)
	{
		--TargetIndex;
	}
	// 2-2. 리스트가 아래로 쭉 땡겨져 있고 리스트 스크롤이 발생하지 않는 경우
	else if (TargetIndex >= RecordCount - 2)
	{
		--TargetIndex;
	}
	// 2-3. 그외의 경우
	else
	{
		--StartIndex;
		--TargetIndex;
	}

	RefreshPage();
}

void UBagUILevel::ScrollDown()
{
	int& StartIndex = StartIndexMemory[Page];
	int& TargetIndex = TargetIndexMemory[Page];
	int RecordCount = UPlayerData::GetRecordCount(PageToItemType(Page));

	// 0. 커서를 더 내릴 수 없는 경우
	if (TargetIndex == RecordCount)
	{
		return;
	}
	
	// 1. RecordCount < 6인 경우 (리스트 스크롤이 발생할 가능성이 아예 없는 경우)
	if (RecordCount < 6)
	{
		++TargetIndex;
	}
	// 2. RecordCount >= 6인 경우 (리스트 스크롤이 발생할 가능성은 있는 경우)
	// 2-1. 리스트가 위로 쭉 땡겨져 있고 리스트 스크롤이 발생하지 않는 경우
	else if (TargetIndex <= 2)
	{
		++TargetIndex;
	}
	// 2-2. 리스트가 아래로 쭉 땡겨져 있고 리스트 스크롤이 발생하지 않는 경우
	else if (TargetIndex >= RecordCount - 2)
	{
		++TargetIndex;
	}
	// 2-3. 그외의 경우
	else
	{
		++StartIndex;
		++TargetIndex;
	}

	RefreshPage();
}

// StartIndex, TargetIndex가 (아이템 삭제로 인해) 잘못 설정되어 있는 경우 고쳐주는 함수 
void UBagUILevel::FixIndexes()
{
	EItemType ItemType = PageToItemType(Page);
	int& StartIndex = StartIndexMemory[Page];
	int& TargetIndex = TargetIndexMemory[Page];
	int RecordCount = UPlayerData::GetRecordCount(ItemType);

	if (TargetIndex < 0)
	{
		TargetIndex = 0;
	}
	else if (TargetIndex > RecordCount)
	{
		TargetIndex = RecordCount;
	}

	// 1. RecordCount < 6인 경우 (스크롤이 없는 경우)
	if(RecordCount < 6)
	{
		StartIndex = 0;
		TargetIndex = UPokemonMath::Min(TargetIndex, RecordCount);
	}
	// 2. RecordCount >= 6인 경우 (스크롤이 있는 경우)
	// 2-1. 제일 위로 당겨져 있는 경우
	else if (TargetIndex <= 2)
	{
		StartIndex = 0;
	}
	// 2-2. 제일 아래로 당겨져 있는 경우
	else if (TargetIndex >= RecordCount - 2)
	{
		StartIndex = RecordCount - 5;
	}
	// 2-3. StartIndex가 정상적으로 설정되어 있는 경우
	else if (TargetIndex - StartIndex == 2 || TargetIndex - StartIndex == 3)
	{
		return;
	}
	// 2-4. StartIndex가 비정상적으로 설정되어 있는 경우
	else
	{
		StartIndex = TargetIndex - 1;
	}
}

void UBagUILevel::RefreshPage()
{
	EItemType ItemType = PageToItemType(Page);
	int& StartIndex = StartIndexMemory[Page];
	int& TargetIndex = TargetIndexMemory[Page];
	int RecordCount = UPlayerData::GetRecordCount(ItemType);

	if (RecordCount == 0)
	{
		Canvas->SetItemImage(RN::UBCloseBag);
		Canvas->SetItemExplain(L"CLOSE BAG");
		Canvas->SetBackground(PageToBackgroundName(Page));
		Canvas->RefreshList({});
		Canvas->SetTargetCursor(0);
		return;
	}

	FixIndexes();

	int EndIndex = UPokemonMath::Min(StartIndex + 5, RecordCount - 1);
	int TargetCursor = TargetIndex - StartIndex;

	std::list<FInventoryRecord> Records = UPlayerData::GetItemList(ItemType, StartIndex, EndIndex);

	if (TargetIndex < RecordCount)
	{
		const FItem* TargetItem = UPlayerData::GetItem(ItemType, TargetIndex);
		Canvas->SetItemImage(TargetItem->ImageName);
		Canvas->SetItemExplain(TargetItem->Explain);
	}
	else
	{
		Canvas->SetItemImage(RN::UBCloseBag);
		Canvas->SetItemExplain(L"CLOSE BAG");
	}

	Canvas->SetBackground(PageToBackgroundName(Page));
	Canvas->RefreshList(Records);
	Canvas->SetTargetCursor(TargetCursor);
}

EItemType UBagUILevel::PageToItemType(int _Page)
{
	return static_cast<EItemType>(_Page + 1);
}

std::string UBagUILevel::PageToBackgroundName(int _Page)
{
	switch (_Page)
	{
	case 0:
		return RN::UBItemsBackground;
	case 1:
		return RN::UBKeyItemsBackground;
	case 2:
		return RN::UBPokeBallsBackground;
	default:
		MsgBoxAssert("가방 UI 페이지가 범위를 벗어납니다.");
		break;
	}

	return "";
}

int UBagUILevel::ItemTypeToPage(EItemType _ItemType)
{
	return static_cast<int>(_ItemType) - 1;
}

const FItem* UBagUILevel::GetTargetItem()
{
	EItemType ItemType = PageToItemType(Page);
	int TargetIndex = TargetIndexMemory[Page];
	return UPlayerData::GetItem(ItemType, TargetIndex);
}

void UBagUILevel::SetItemUsage(EItemUsage _Usage)
{
	ItemUsage = _Usage;
}