#include "ItemPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "BagUILevel.h"
#include "SoundManager.h"

const EItemPokemonUIState EItemPokemonUIState::TestItemUse;
const EItemPokemonUIState EItemPokemonUIState::HpUpEffect;
const EItemPokemonUIState EItemPokemonUIState::ItemUseResultMessage;

UItemPokemonUILevel::UItemPokemonUILevel()
{
}

UItemPokemonUILevel::~UItemPokemonUILevel()
{
}

void UItemPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonUILevel::LevelStart(_PrevLevel);

	BagUILevel = dynamic_cast<UBagUILevel*>(_PrevLevel);

	if (nullptr == BagUILevel)
	{
		MsgBoxAssert("BagItemPokemonUILevel�� ���� ������ UBagUILevel�� �ƴմϴ�. ���� ����: " + _PrevLevel->GetName());
		return;
	}

	State = EItemPokemonUIState::TargetSelectionWait;
	UseItem = BagUILevel->GetTargetItem();
	Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBoxBagMode);
}

void UItemPokemonUILevel::Tick(float _DeltaTime)
{
	UPokemonUILevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	if (State == EItemPokemonUIState::TargetSelectionWait)
	{
		ProcessTargetSelectionWait();
	}
	else if (State == EItemPokemonUIState::ActionSelectionWait)
	{
		ProcessActionSelectionWait();
	}
	else if (State == EItemPokemonUIState::TestItemUse)
	{
		ProcessTestItemUse();
	}
	else if (State == EItemPokemonUIState::HpUpEffect)
	{
		ProcessHpUpEffect();
	}
	else if (State == EItemPokemonUIState::ItemUseResultMessage)
	{
		ProcessItemUseResultMessage();
	}
}


void UItemPokemonUILevel::ProcessTestItemUse()
{
	UPokemon* SelectedPokemon = &UUserData::GetPokemonInEntry(TargetCursor);
	EUseEffect UseEffect = UseItem->UseEffect;

	if (EUseEffect::Hp == UseEffect)
	{
		int CurHp = SelectedPokemon->GetCurHp();
		int MaxHp = SelectedPokemon->GetHp();

		// Ǯ���� ���
		if (CurHp == MaxHp)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// ������ ��뿡 ������ ��� ������ ��뿡 �����ߴٰ� ��ŷ�صд�.
			ItemUseResult = false;
			return;
		}

		PrevHealHp = CurHp;
		NextHealHp = UPokemonMath::Min(MaxHp, CurHp + UseItem->HealValue);
		SelectedPokemon->SetCurHp(NextHealHp);

		State = EItemPokemonUIState::HpUpEffect;
		Timer = HealTime;
	}
	else if (EUseEffect::CureAll == UseEffect)
	{
		// ���� ���� �Ǵ� ���� ������ ���
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Normal == StatusId || EPokemonStatus::Faint == StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// ������ ��뿡 ������ ��� ������ ��뿡 �����ߴٰ� ��ŷ�صд�.
			ItemUseResult = false;
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" became healthy.");
		Canvas->RefreshAllTargets();
	}
	else if (EUseEffect::CureBurn == UseEffect)
	{
		// ȭ�� ���°� �ƴ� ���
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Burn != StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// ������ ��뿡 ������ ��� ������ ��뿡 �����ߴٰ� ��ŷ�صд�.
			ItemUseResult = false;
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" was cured of it's burning.");
		Canvas->RefreshAllTargets();
	}
	else if (EUseEffect::CurePoison == UseEffect)
	{
		// �ߵ� ���°� �ƴ� ���
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Poison != StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// ��Ʋ ������ ���� ��忡�� ������ ��뿡 ������ ��� ������ ��뿡 �����ߴٰ� ��ŷ�صд�.
			ItemUseResult = false;
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" was cured of it's poisoning.");
		Canvas->RefreshAllTargets();
	}

	// ������ ��� ����
	ItemUseResult = true;
	USoundManager::StopSE();
	USoundManager::PlaySE(RN::SERecover);
	UUserData::LoseItem(UseItem->Id);
}

void UItemPokemonUILevel::ProcessHpUpEffect()
{
	const UPokemon* SelectedPokemon = &UUserData::GetPokemonInEntry(TargetCursor);
	Canvas->LerpHeal(TargetCursor, PrevHealHp, NextHealHp, SelectedPokemon->GetHp(), Timer / HealTime);

	if (Timer <= 0.0f)
	{
		State = EItemPokemonUIState::ItemUseResultMessage;

		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L"'s HP was restored\nby " + std::to_wstring(NextHealHp - PrevHealHp) + L" points.");
	}
}

void UItemPokemonUILevel::SelectTarget()
{
	// ��� ��ư�� ������ ���
	if (TargetCursor == UUserData::GetPokemonEntrySize())
	{
		CancelTargetSelection();
		return;
	}

	State = EItemPokemonUIState::TestItemUse;
}
