#include "BattleShiftPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonSummaryUILevel.h"
#include "BattleLevel.h"

// ���ϸ��� ������ �ְų� �̹� ���� �ִٴ� �޽����� ���� �ִ� ����
const EBattleShiftPokemonUIState EBattleShiftPokemonUIState::BattleShiftFailMessageShow;

UBattleShiftPokemonUILevel::UBattleShiftPokemonUILevel() 
{
}

UBattleShiftPokemonUILevel::~UBattleShiftPokemonUILevel() 
{
}

void UBattleShiftPokemonUILevel::Tick(float _DeltaTime)
{
	UPokemonUILevel::Tick(_DeltaTime);

	if (State == EBattleShiftPokemonUIState::TargetSelectionWait)
	{
		ProcessTargetSelectionWait();
	}
	else if (State == EBattleShiftPokemonUIState::ActionSelectionWait)
	{
		ProcessActionSelectionWait();
	}
	else if (State == EBattleShiftPokemonUIState::BattleShiftFailMessageShow)
	{
		ProcessBattleShiftFailMessageShow();
	}
}

void UBattleShiftPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonUILevel::LevelStart(_PrevLevel);

	// ���� ������ SummaryUI ������ ��� ���� �ʱ�ȭ�� ���� �ʴ´�.
	UPokemonSummaryUILevel* SummaryUILevel = dynamic_cast<UPokemonSummaryUILevel*>(_PrevLevel);

	if (nullptr != SummaryUILevel)
	{
		return;
	}

	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("UBattleShiftPokemonUILevel�� ���� ������ UBattleLevel�� �ƴմϴ�.");
		return;
	}

	State = EBattleShiftPokemonUIState::TargetSelectionWait;
	PlayerBattler = BattleLevel->GetPlayerBattler();
	PlayerBattler->SetShiftPokemonIndex(PlayerBattler->CurPokemonIndex());
	Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBox);
	Canvas->SetActionBoxImage(RN::PokemonUIBattleActionBox);
}

void UBattleShiftPokemonUILevel::ProcessBattleShiftFailMessageShow()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EBattleShiftPokemonUIState::TargetSelectionWait;
		Canvas->SetCustomMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
	}
}

void UBattleShiftPokemonUILevel::SelectTarget()
{
	// ��� ��ư�� ������ ���
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		CancelTargetSelection();
		return;
	}

	State = EBattleShiftPokemonUIState::ActionSelectionWait;
	Canvas->SetTargetSelectionMsgBoxActive(false);
	Canvas->SetActionSelectionMsgBoxActive(true);
	Canvas->SetActionBoxActive(true);
	Canvas->SetActionCursor(0);
}

void UBattleShiftPokemonUILevel::CancelTargetSelection()
{
	// ��Ʋ �� �÷��̾� ���ϸ��� �����ؼ� ������ ���ϸ� ������ �� ��� Cancel ������ �Ұ����ϴ�.
	if (true == PlayerBattler->CurPokemon()->IsFaint())
	{
		State = EBattleShiftPokemonUIState::TargetSelectionWait;
	}
	// �� ���� ��� ��Ʋ ������ ���ư���.
	else
	{
		UEventManager::FadeChangeLevel(Global::BattleLevel, false);
	}
}


void UBattleShiftPokemonUILevel::SelectAction()
{
	switch (Canvas->GetActionCursor())
	{
	case 0:
	{
		// Send Out
		const UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
		if (SelectedPokemon == PlayerBattler->CurPokemon())
		{
			// ��Ʋ�� ���� �ִ� ���ϸ��� ���� ���
			State = EBattleShiftPokemonUIState::BattleShiftFailMessageShow;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetActionBoxActive(false);
			Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + +L" is already\nin battle!");
			return;
		}
		else if (true == SelectedPokemon->IsFaint())
		{
			// ������ ���ϸ��� ���� ���
			State = EBattleShiftPokemonUIState::BattleShiftFailMessageShow;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetActionBoxActive(false);
			Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" has no energy\nleft to battle!");
			return;
		}

		UEventManager::FadeChangeLevel(Global::BattleLevel, false);
		PlayerBattler->SetShiftPokemonIndex(TargetCursor);
	}
	break;
	case 1:
		// PokemonSummaryUI ������ ��ȯ
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 2:
		// Target ���·� ����
		State = EBattleShiftPokemonUIState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
		break;
	default:
		break;
	}
}