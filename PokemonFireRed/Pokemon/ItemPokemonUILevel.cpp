#include "ItemPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "BagUILevel.h"

const EItemPokemonUIState EItemPokemonUIState::TestItemUse;
const EItemPokemonUIState EItemPokemonUIState::HpUpEffect;
const EItemPokemonUIState EItemPokemonUIState::ItemUseResultMessage;

UItemPokemonUILevel::UItemPokemonUILevel()
{
}

UItemPokemonUILevel::~UItemPokemonUILevel()
{
}

const FItem* UItemPokemonUILevel::GetUseItem() const
{
	return UseItem;
}

void UItemPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonUILevel::LevelStart(_PrevLevel);

	BagUILevel = dynamic_cast<UBagUILevel*>(_PrevLevel);

	if (nullptr == BagUILevel)
	{
		MsgBoxAssert("BagItemPokemonUILevel의 이전 레벨이 UBagUILevel이 아닙니다. 이전 레벨: " + _PrevLevel->GetName());
		return;
	}

	Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBoxBagMode);
}


void UItemPokemonUILevel::ProcessTestItemUse()
{
	UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
	EUseEffect UseEffect = UseItem->UseEffect;

	if (EUseEffect::Hp == UseEffect)
	{
		int CurHp = SelectedPokemon->GetCurHp();
		int MaxHp = SelectedPokemon->GetHp();

		// 풀피인 경우
		if (CurHp == MaxHp)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// 아이템 사용에 실패한 경우 아이템 사용에 실패했다고 마킹해둔다.
			BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::NotUsed);
			return;
		}

		PrevHealHp = CurHp;
		NextHealHp = UPokemonMath::Min(MaxHp, CurHp + UseItem->HealValue);
		SelectedPokemon->SetCurHp(NextHealHp);

		State = EItemPokemonUIState::HpUpEffect;
		Timer = HealTime;

		//아이템 사용에 성공한 경우 아이템을 사용했다고 마킹해둔다.
		BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::Used);
	}
	else if (EUseEffect::CureAll == UseEffect)
	{
		// 정상 상태 또는 기절 상태인 경우
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Normal == StatusId || EPokemonStatus::Faint == StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// 아이템 사용에 실패한 경우 아이템 사용에 실패했다고 마킹해둔다.
			BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::NotUsed);
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" became healthy.");
		Canvas->RefreshAllTargets();

		// 아이템 사용에 성공한 경우 바로 
		BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::Used);
	}
	else if (EUseEffect::CureBurn == UseEffect)
	{
		// 화상 상태가 아닌 경우
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Burn != StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// 아이템 사용에 실패한 경우 아이템 사용에 실패했다고 마킹해둔다.
			BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::NotUsed);
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" was cured of it's burning.");
		Canvas->RefreshAllTargets();

		// 아이템 사용에 성공한 경우 아이템을 사용했다고 마킹해둔다.
		BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::Used);
	}
	else if (EUseEffect::CurePoison == UseEffect)
	{
		// 중독 상태가 아닌 경우
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Poison != StatusId)
		{
			State = EItemPokemonUIState::ItemUseResultMessage;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");

			// 배틀 아이템 선택 모드에서 아이템 사용에 실패한 경우 아이템 사용에 실패했다고 마킹해둔다.
			BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::NotUsed);
			return;
		}

		State = EItemPokemonUIState::ItemUseResultMessage;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" was cured of it's poisoning.");
		Canvas->RefreshAllTargets();

		// 배틀 아이템 선택 모드에서 아이템 사용에 성공한 경우 아이템을 사용했다고 마킹해둔다.
		BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::Used);
	}
}

void UItemPokemonUILevel::ProcessHpUpEffect()
{
	const UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
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
	// 취소 버튼을 선택한 경우
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		CancelTargetSelection();
		return;
	}

	State = EItemPokemonUIState::TestItemUse;
}

void UItemPokemonUILevel::CancelTargetSelection()
{
	BagUILevel->SetItemUsage(UBagUILevel::EItemUsage::NotUsed);
	UEventManager::FadeChangeLevel(Global::BagUILevel, false);
	Canvas->SetActionCursor(0);
}
