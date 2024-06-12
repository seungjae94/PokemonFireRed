#include "BattleShiftPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonSummaryUILevel.h"
#include "BattleLevel.h"

// 포켓몬이 기절해 있거나 이미 나와 있다는 메시지를 띄우고 있는 상태
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

	// 이전 레벨이 SummaryUI 레벨인 경우 레벨 초기화를 하지 않는다.
	UPokemonSummaryUILevel* SummaryUILevel = dynamic_cast<UPokemonSummaryUILevel*>(_PrevLevel);

	if (nullptr != SummaryUILevel)
	{
		return;
	}

	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("UBattleShiftPokemonUILevel의 이전 레벨이 UBattleLevel이 아닙니다.");
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
	// 취소 버튼을 선택한 경우
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
	// 배틀 중 플레이어 포켓몬이 기절해서 강제로 포켓몬 레벨로 온 경우 Cancel 선택이 불가능하다.
	if (true == PlayerBattler->CurPokemon()->IsFaint())
	{
		State = EBattleShiftPokemonUIState::TargetSelectionWait;
	}
	// 그 외의 경우 배틀 레벨로 돌아간다.
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
			// 배틀에 나와 있는 포켓몬을 고르는 경우
			State = EBattleShiftPokemonUIState::BattleShiftFailMessageShow;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetActionBoxActive(false);
			Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + +L" is already\nin battle!");
			return;
		}
		else if (true == SelectedPokemon->IsFaint())
		{
			// 기절한 포켓몬을 고르는 경우
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
		// PokemonSummaryUI 레벨로 전환
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 2:
		// Target 상태로 복귀
		State = EBattleShiftPokemonUIState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
		break;
	default:
		break;
	}
}