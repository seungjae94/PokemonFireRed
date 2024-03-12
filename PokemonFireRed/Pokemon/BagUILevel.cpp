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

	// ���ϸ� UI �������� �ǵ��ƿ��� ��� ������ �ʱ�ȭ���� �ʴ´�.
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

		// ��� ��ư�� ���� ���
		if (TargetIndex == RecordCount)
		{
			// ��Ʋ ��忡�� �ǵ��ư��� ��� ������ ����� ����ߴٰ� ��ŷ�� ����� �Ѵ�.
			if (true == BattleMode)
			{
				PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
			}
			UEventManager::FadeChangeLevel(PrevLevelName);
			return;
		}

		// ��Ʋ ��尡 �ƴ� ��, �� �� �������� ���� ���� ����� �� ����.
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
		// ��Ʋ ��忡�� �ǵ��ư��� ��� ������ ����� ����ߴٰ� ��ŷ�� ����� �Ѵ�.
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

		// ��� �׼��� ������ ��� �׼�â�� ���� �ٽ� �������� �����Ѵ�.
		if (Cursor == 1)
		{
			State = EState::TargetSelect;
			Canvas->SetActionCursor(0);
			Canvas->SetActionItemBoxActive(false);
			return;
		}
		
		// ��Ʋ ��忡�� ���� ���� ������ ���
		if (true == BattleMode && PageToItemType(Page) == EItemType::PokeBall)
		{
			State = EState::TargetSelect;
			PlayerBattler->SetItemSelectState(EItemSelectState::BallSelected);
			UEventManager::FadeChangeLevel(PrevLevelName);
		}
		// ��Ʋ ��忡�� �Һ� �������� ������ ���
		else if (true == BattleMode && PageToItemType(Page) == EItemType::UseItem)
		{
			State = EState::BattleModeItemUsageCheck;
			ItemUsage = EItemUsage::None;
			UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		}
		// ��Ʋ ��尡 �ƴϰ� �Һ� �������� ���������� ���ϸ��� ���� ���
		else if (UPlayerData::GetPokemonEntrySize() == 0)
		{
			State = EState::TargetSelect;
		}
		// ��Ʋ ��尡 �ƴϰ� �Һ� �������� ���������� ���ϸ��� �ִ� ���
		else
		{
			State = EState::TargetSelect;
			UEventManager::FadeChangeLevel(Global::PokemonUILevel);
		}

		// �׼��� ��ġ�� ���� �׼�â�� ���д�.
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		return;
	}

	// �׼�â�� ���� �ٽ� �������� �����Ѵ�.
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
	// �Һ� �������� ����ߴ�.
	if (EItemUsage::Used == ItemUsage)
	{
		State = EState::TargetSelect;
		PlayerBattler->SetItemSelectState(EItemSelectState::ItemUsed);
		UEventManager::FadeChangeLevel(PrevLevelName);
	}
	// �Һ� �������� ������� �ʾҴ�. �ٽ� �������� �����Ѵ�.
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

	// 0. Ŀ���� �� �ø� �� ���� ���
	if (TargetIndex == 0)
	{
		return;
	}

	// 1. RecordCount < 6�� ��� (����Ʈ ��ũ���� �߻��� ���ɼ��� �ƿ� ���� ���)
	if (RecordCount < 6)
	{
		--TargetIndex;
	}
	// 2. RecordCount >= 6�� ��� (����Ʈ ��ũ���� �߻��� ���ɼ��� �ִ� ���)
	// 2-1. ����Ʈ�� ���� �� ������ �ְ� ����Ʈ ��ũ���� �߻����� �ʴ� ���
	else if (TargetIndex <= 2)
	{
		--TargetIndex;
	}
	// 2-2. ����Ʈ�� �Ʒ��� �� ������ �ְ� ����Ʈ ��ũ���� �߻����� �ʴ� ���
	else if (TargetIndex >= RecordCount - 2)
	{
		--TargetIndex;
	}
	// 2-3. �׿��� ���
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

	// 0. Ŀ���� �� ���� �� ���� ���
	if (TargetIndex == RecordCount)
	{
		return;
	}
	
	// 1. RecordCount < 6�� ��� (����Ʈ ��ũ���� �߻��� ���ɼ��� �ƿ� ���� ���)
	if (RecordCount < 6)
	{
		++TargetIndex;
	}
	// 2. RecordCount >= 6�� ��� (����Ʈ ��ũ���� �߻��� ���ɼ��� �ִ� ���)
	// 2-1. ����Ʈ�� ���� �� ������ �ְ� ����Ʈ ��ũ���� �߻����� �ʴ� ���
	else if (TargetIndex <= 2)
	{
		++TargetIndex;
	}
	// 2-2. ����Ʈ�� �Ʒ��� �� ������ �ְ� ����Ʈ ��ũ���� �߻����� �ʴ� ���
	else if (TargetIndex >= RecordCount - 2)
	{
		++TargetIndex;
	}
	// 2-3. �׿��� ���
	else
	{
		++StartIndex;
		++TargetIndex;
	}

	RefreshPage();
}

// StartIndex, TargetIndex�� (������ ������ ����) �߸� �����Ǿ� �ִ� ��� �����ִ� �Լ� 
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

	// 1. RecordCount < 6�� ��� (��ũ���� ���� ���)
	if(RecordCount < 6)
	{
		StartIndex = 0;
		TargetIndex = UPokemonMath::Min(TargetIndex, RecordCount);
	}
	// 2. RecordCount >= 6�� ��� (��ũ���� �ִ� ���)
	// 2-1. ���� ���� ����� �ִ� ���
	else if (TargetIndex <= 2)
	{
		StartIndex = 0;
	}
	// 2-2. ���� �Ʒ��� ����� �ִ� ���
	else if (TargetIndex >= RecordCount - 2)
	{
		StartIndex = RecordCount - 5;
	}
	// 2-3. StartIndex�� ���������� �����Ǿ� �ִ� ���
	else if (TargetIndex - StartIndex == 2 || TargetIndex - StartIndex == 3)
	{
		return;
	}
	// 2-4. StartIndex�� ������������ �����Ǿ� �ִ� ���
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
		MsgBoxAssert("���� UI �������� ������ ����ϴ�.");
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