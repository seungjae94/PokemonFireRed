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

	// ���ϸ� UI �������� �ǵ��ƿ��� ��� ������ �ʱ�ȭ���� �ʴ´�.
	if (nullptr != PokemonUILevel)
	{
		return;
	}

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);

	if (nullptr == MapLevel)
	{
		MsgBoxAssert("UMenuBagUILevel�� ���� ������ �� ������ �ƴմϴ�. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PrevMapName = MapLevel->GetName();
}


void UMenuBagUILevel::SelectTarget()
{
	int RecordCount = UUserData::GetRecordCount(GetCurItemType());
	int TargetIndex = TargetIndexMemory[Page];

	// ��� ��ư�� ���� ���
	if (TargetIndex == RecordCount)
	{
		UEventManager::FadeChangeLevel(PrevMapName);
		return;
	}

	// �� �������� ���� ���� ����� �� ����.
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

	// ��� �׼��� ������ ��� �׼�â�� ���� �ٽ� �������� �����Ѵ�.
	if (Cursor == 1)
	{
		State = EBagUIState::TargetSelect;
		Canvas->SetActionCursor(0);
		Canvas->SetActionItemBoxActive(false);
		return;
	}

	// ���ϸ��� ���� ��� �׼� ������ ���Ƶд�.
	if (UUserData::GetPokemonEntrySize() == 0)
	{
		State = EBagUIState::TargetSelect;
	}
	// ���ϸ��� �ִ� ��� ���ϸ� �������� ����.
	else
	{
		State = EBagUIState::TargetSelect;
		UEventManager::FadeChangeLevel(Global::BagItemPokemonUILevel);
	}

	// �׼��� ��ġ�� ���� �׼�â�� ���д�.
	Canvas->SetActionCursor(0);
	Canvas->SetActionItemBoxActive(false);
}

