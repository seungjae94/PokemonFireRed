#include "MenuPokemonUILevel.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonSummaryUILevel.h"
#include "MapLevel.h"

const EMenuPokemonUIState EMenuPokemonUIState::ActionSelectionWait;
const EMenuPokemonUIState EMenuPokemonUIState::SwitchSelectionWait;
const EMenuPokemonUIState EMenuPokemonUIState::SwitchMoveOut;
const EMenuPokemonUIState EMenuPokemonUIState::SwitchMoveWait;
const EMenuPokemonUIState EMenuPokemonUIState::SwitchMoveIn;

UMenuPokemonUILevel::UMenuPokemonUILevel()
{
}

UMenuPokemonUILevel::~UMenuPokemonUILevel()
{
}

void UMenuPokemonUILevel::Tick(float _DeltaTime)
{
	UPokemonUILevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	if (State == EMenuPokemonUIState::TargetSelectionWait)
	{
		ProcessTargetSelectionWait();
	}
	else if (State == EMenuPokemonUIState::ActionSelectionWait)
	{
		ProcessActionSelectionWait();
	}
	else if (State == EMenuPokemonUIState::SwitchSelectionWait)
	{
		ProcessSwitchSelectionWait();
	}
	else if (State == EMenuPokemonUIState::SwitchMoveOut)
	{
		ProcessSwitchMoveOut();
	}
	else if (State == EMenuPokemonUIState::SwitchMoveWait)
	{
		ProcessSwitchMoveWait();
	}
	else if (State == EMenuPokemonUIState::SwitchMoveIn)
	{
		ProcessSwitchMoveIn();
	}
}

void UMenuPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonUILevel::LevelStart(_PrevLevel);

	// 이전 레벨이 SummaryUI 레벨인 경우 레벨 초기화를 하지 않는다.
	UPokemonSummaryUILevel* SummaryUILevel = dynamic_cast<UPokemonSummaryUILevel*>(_PrevLevel);

	if (nullptr != SummaryUILevel)
	{
		return;
	}

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);

	if (nullptr == MapLevel)
	{
		MsgBoxAssert("UMenuPokemonUILevel의 이전 레벨이 맵 레벨이 아닙니다. PrevLevel: " + _PrevLevel->GetName());
		return;
	}

	State = EPokemonUIState::TargetSelectionWait;
	MapLevelName = _PrevLevel->GetName();
	Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBox);
}

void UMenuPokemonUILevel::SelectAction()
{
	switch (Canvas->GetActionCursor())
	{
	case 0:
		// PokemonSummaryUI 레벨로 전환
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 1:
		// Switch 상태로 전환
		State = EMenuPokemonUIState::SwitchSelectionWait;
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetSwitchSelectionMsgBoxActive(true);
		Canvas->SetActionBoxActive(false);
		SwitchFromCursor = TargetCursor;
		Canvas->RefreshAllTargets();
		break;
	case 2:
		// Target 상태로 복귀
		State = EMenuPokemonUIState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
		break;
	default:
		break;
	}
}

void UMenuPokemonUILevel::ProcessSwitchSelectionWait()
{
	if (true == UEngineInput::IsDown('X'))
	{
		PlaySEClick();
		State = EMenuPokemonUIState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		PlaySEClick();
		SelectSwitch();
		return;
	}

	int EntrySize = UPlayerData::GetPokemonEntrySize();
	if (true == UEngineInput::IsDown(VK_LEFT)
		&& true == IsTargetCursorOnEntry())
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = 0;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_RIGHT)
		&& true == IsTargetCursorOnFirst()
		&& EntrySize > 1)
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = 1;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
	}
}


void UMenuPokemonUILevel::SelectSwitch()
{
	if (TargetCursor == SwitchFromCursor
		|| true == IsTargetCursorOnCancel())
	{
		// 스위치 취소
		State = EMenuPokemonUIState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	State = EMenuPokemonUIState::SwitchMoveOut;
	Timer = SwitchMoveOutTime;
	SwitchFromBox = Canvas->GetPokemonBox(SwitchFromCursor);
	SwitchFromInPos = SwitchFromBox->GetRelativePosition();
	if (true == Canvas->IsFirstBox(SwitchFromBox))
	{
		SwitchFromOutPos = SwitchFromInPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchFromOutPos = SwitchFromInPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}
	SwitchToBox = Canvas->GetPokemonBox(TargetCursor);
	SwitchToInPos = SwitchToBox->GetRelativePosition();
	if (true == Canvas->IsFirstBox(SwitchToBox))
	{
		SwitchToOutPos = SwitchToInPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchToOutPos = SwitchToInPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}
}


void UMenuPokemonUILevel::ProcessSwitchMoveOut()
{
	float t = Timer / SwitchMoveOutTime;
	SwitchFromBox->SetRelativePosition(UPokemonMath::Lerp(SwitchFromOutPos, SwitchFromInPos, t));
	SwitchToBox->SetRelativePosition(UPokemonMath::Lerp(SwitchToOutPos, SwitchToInPos, t));

	if (Timer <= 0.0f)
	{
		State = EMenuPokemonUIState::SwitchMoveWait;
		Timer = SwitchMoveWaitTime;

		UPlayerData::SwapEntry(SwitchFromCursor, TargetCursor);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::To);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::From);

		Canvas->RefreshAllTargets(true);
	}
}

void UMenuPokemonUILevel::ProcessSwitchMoveWait()
{
	if (Timer <= 0.0f)
	{
		State = EMenuPokemonUIState::SwitchMoveIn;
		Timer = SwitchMoveInTime;
	}
}

void UMenuPokemonUILevel::ProcessSwitchMoveIn()
{
	float t = Timer / SwitchMoveInTime;
	SwitchFromBox->SetRelativePosition(UPokemonMath::Lerp(SwitchFromInPos, SwitchFromOutPos, t));
	SwitchToBox->SetRelativePosition(UPokemonMath::Lerp(SwitchToInPos, SwitchToOutPos, t));

	if (Timer <= 0.0f)
	{
		State = EMenuPokemonUIState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);

		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);

		Canvas->RefreshAllTargets();
	}
}

void UMenuPokemonUILevel::SelectTarget()
{
	// 취소 버튼을 선택한 경우
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		CancelTargetSelection();
		return;
	}

	State = EMenuPokemonUIState::ActionSelectionWait;
	Canvas->SetTargetSelectionMsgBoxActive(false);
	Canvas->SetActionSelectionMsgBoxActive(true);
	Canvas->SetActionBoxActive(true);
	Canvas->SetActionCursor(0);
}

void UMenuPokemonUILevel::CancelTargetSelection()
{
	UEventManager::FadeChangeLevel(MapLevelName, false);
	Canvas->SetActionCursor(0);
}
