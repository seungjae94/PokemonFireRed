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

	// ���ϸ� UI �������� �ǵ��ƿ��� ��� ������ �ʱ�ȭ���� �ʴ´�.
	if (nullptr != PokemonUILevel)
	{
		return;
	}


	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("UBattleBagUILevel�� ���� ������ ��Ʋ ������ �ƴմϴ�. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	PlayerBattler = BattleLevel->GetPlayerBattler();
}

void UBattleBagUILevel::SelectTarget()
{
	int RecordCount = UPlayerData::GetRecordCount(GetCurItemType());
	int TargetIndex = TargetIndexMemory[Page];

	// ��� ��ư�� ���� ���
	if (TargetIndex == RecordCount)
	{
		// ������ ����� ����ߴٰ� ��ŷ�� ����� �Ѵ�.
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
	// ������ ����� ����ߴٰ� ��ŷ�� ����� �Ѵ�.
	PlayerBattler->SetItemSelectState(EItemSelectState::Canceled);
	UEventManager::FadeChangeLevel(Global::BattleLevel);
}

void UBattleBagUILevel::SelectAction()
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

	// ���� ���� ������ ��� ������ ��� ����� �÷��̾� ��Ʋ���� �־��ְ� ��Ʋ ������ ���ư���.
	if (GetCurItemType() == EItemType::PokeBall)
	{
		State = EBagUIState::TargetSelect;
		PlayerBattler->SelectItem(GetTargetItem());
		PlayerBattler->SetItemSelectState(EItemSelectState::BallSelected);
		UEventManager::FadeChangeLevel(Global::BattleLevel);
	}
	// �Һ� �������� ������ ��� ������ ��� ����� �÷��̾� ��Ʋ���� �־��ִ� �� UBattleItemPokemonUILevel�� ����Ѵ�.
	else if (GetCurItemType() == EItemType::UseItem)
	{
		State = EBagUIState::TargetSelect;
		UEventManager::FadeChangeLevel(Global::BattleItemPokemonUILevel);
	}

	// �׼��� ��ġ�� ���� �׼�â�� ���д�.
	Canvas->SetActionCursor(0);
	Canvas->SetActionItemBoxActive(false);
}
