#include "BagUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "MapLevel.h"
#include "BattleLevel.h"
#include "PokemonUILevel.h"
#include "Battler.h"

const EBagUIState EBagUIState::None;
const EBagUIState EBagUIState::TargetSelect;
const EBagUIState EBagUIState::ActionSelect;
int EBagUIState::MaxIndex = 0;

bool UBagUILevel::IsCommonResourcesLoaded = false;

void EBagUIState::operator=(const EBagUIState& _Other)
{
	Index = _Other.Index;
}

bool EBagUIState::operator==(const EBagUIState& _Other)
{
	return Index == _Other.Index;
}

bool EBagUIState::operator<(const EBagUIState& _Other)
{
	return Index < _Other.Index;
}

bool EBagUIState::operator>(const EBagUIState& _Other)
{
	return Index > _Other.Index;
}


UBagUILevel::UBagUILevel()
{
}

UBagUILevel::~UBagUILevel()
{
}

void UBagUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	if (false == IsCommonResourcesLoaded)
	{
		UEngineDirectory CurDir;
		CurDir.MoveToSearchChild("Resources");
		CurDir.Move("BagUILevel");

		std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
		for (UEngineFile& File : Files)
		{
			UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
		}

		IsCommonResourcesLoaded = true;
	}
	
	Canvas = SpawnActor<ABagCanvas>();
}

void UBagUILevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	if (State == EBagUIState::TargetSelect)
	{
		ProcessTargetSelect();
	}
	else if (State == EBagUIState::ActionSelect)
	{
		ProcessActionSelect();
	}
}

void UBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	UPokemonUILevel* PokemonUILevel = dynamic_cast<UPokemonUILevel*>(_PrevLevel);

	RefreshPage();

	// ���ϸ� UI �������� �ǵ��ƿ��� ��� ����Ʈ�� �ʱ�ȭ�Ѵ�.
	if (nullptr != PokemonUILevel)
	{
		return;
	}

	State = EBagUIState::TargetSelect;
	Canvas->SetActionItemBoxActive(false);
}

void UBagUILevel::ProcessTargetSelect()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		SelectTarget();
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		CancelTargetSelection();
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
		SelectAction();
		return;
	}

	// �׼�â�� ���� �ٽ� �������� �����Ѵ�.
	if (true == UEngineInput::IsDown('X'))
	{
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		State = EBagUIState::TargetSelect;
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

EItemType UBagUILevel::GetCurItemType() const
{
	return static_cast<EItemType>(Page + 1);
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