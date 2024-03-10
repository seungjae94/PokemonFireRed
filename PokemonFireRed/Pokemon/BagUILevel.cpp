#include "BagUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UBagUILevel::UBagUILevel()
{
}

UBagUILevel::~UBagUILevel()
{
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

	

}

void UBagUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	PrevLevelName = _PrevLevel->GetName();

	State = EState::TargetSelect;

	// 위에서부터 그릴 데이터를 최대 6개까지 제공해야 함
	EItemType ItemType = PageToItemType(Page);
	int& StartIndex = StartIndexMemory[Page];
	int EndIndex = 0;
	int& TargetIndex = TargetIndexMemory[Page];
	int TargetCursor = 0;
	int RecordCount = UPlayerData::GetRecordCount(ItemType);

	// 1. RecordCount < 6인 경우 (스크롤이 없는 경우)
	if (RecordCount < 6)
	{
		StartIndex = 0;
		EndIndex = RecordCount - 1;
		TargetIndex = UPokemonMath::Min(TargetIndex, RecordCount);
		TargetCursor = TargetIndex;
	}
	// 2. RecordCount >= 6인 경우 (스크롤이 있는 경우)
	// 2-1. 제일 위로 당겨져 있는 경우
	else if (TargetIndex <= 3)
	{
		StartIndex = 0;
		EndIndex = 5;
		TargetCursor = TargetIndex;
	}
	// 2-2. 제일 아래로 당겨져 있는 경우
	else if (TargetIndex >= RecordCount - 3)
	{
		StartIndex = RecordCount - 5;
		EndIndex = RecordCount;
		TargetCursor = TargetIndex - StartIndex;
	}
	// 2-3. 그외의 경우
	else
	{
		EndIndex = StartIndex + 5;
		TargetCursor = TargetIndex - StartIndex;
	}

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

void UBagUILevel::LevelEnd(ULevel* _NextLevel)
{
	UPokemonLevel::LevelEnd(_NextLevel);

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
