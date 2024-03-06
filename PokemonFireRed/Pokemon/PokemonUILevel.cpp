#include "PokemonUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonCanvas.h"
#include "MapLevel.h"
#include "BattleLevel.h"
#include "PokemonSummaryUILevel.h"
#include "BattleUtil.h"

UPokemonUILevel::UPokemonUILevel()
{
}

UPokemonUILevel::~UPokemonUILevel()
{
}

void UPokemonUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("PokemonUILevel");

	std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	Canvas = SpawnActor<APokemonCanvas>();
}

void UPokemonUILevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

	switch (State)
	{
	case EState::TargetSelectionWait:
		ProcessTargetSelectionWait();
		break;
	case EState::ActionSelectionWait:
		ProcessActionSelectionWait();
		break;
	case EState::BattleActionSelectionWait:
		ProcessBattleActionSelectionWait();
		break;
	case EState::BattleShiftFailMessageShow:
		ProcessBattleShiftFailMessageShow();
		break;
	case EState::SwitchSelectionWait:
		ProcessSwitchSelectionWait();
		break;
	case EState::SwitchMoveOut:
		ProcessSwitchMoveOut();
		break;
	case EState::SwitchMoveWait:
		ProcessSwitchMoveWait();
		break;
	case EState::SwitchMoveIn:
		ProcessSwitchMoveIn();
		break;
	default:
		break;
	}
}

void UPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	// ���� ���� �з�
	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);
	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(_PrevLevel);
	UPokemonSummaryUILevel* SummaryUILevel = dynamic_cast<UPokemonSummaryUILevel*>(_PrevLevel);

	if (nullptr != SummaryUILevel)
	{
		// ���� ������ SummaryUI ������ ��� ���� �ʱ�ȭ�� ���� �ʴ´�.
		return;
	}

	// ���� �� ���
	PrevLevelName = _PrevLevel->GetName();

	// ���� �ʱ�ȭ
	Canvas->Init();
	TargetCursor = 0;
	State = EState::TargetSelectionWait;

	// �� ���� ó��
	if (nullptr != MapLevel)
	{
		BattleMode = false;
	}
	// ��Ʋ ���� ó��
	else if (nullptr != BattleLevel)
	{
		BattleMode = true;
		PlayerBattler = BattleLevel->GetPlayerBattler();
		PlayerBattler->SetShiftPokemonIndex(PlayerBattler->CurPokemonIndex());
	}
}

void UPokemonUILevel::ProcessTargetSelectionWait()
{
	int EntrySize = UPlayerData::GetPokemonEntrySize();

	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevLevelName, false);
		Canvas->SetActionCursor(0);
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		SelectTarget();
		return;
	}

	if (true == UEngineInput::IsDown(VK_LEFT)
		&& true == IsTargetCursorOnEntry())
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = 0;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_RIGHT)
		&& true == IsTargetCursorOnFirst()
		&& EntrySize > 1)
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = 1;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
	}
}

void UPokemonUILevel::ProcessActionSelectionWait()
{
	if (UEngineInput::IsDown(VK_UP))
	{
		Canvas->DecActionCursor();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->IncActionCursor();
	}
	else if (UEngineInput::IsDown('X'))
	{
		State = EState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
	}
	else if (UEngineInput::IsDown('Z'))
	{
		SelectAction();
	}
}

void UPokemonUILevel::ProcessBattleActionSelectionWait()
{
	if (UEngineInput::IsDown(VK_UP))
	{
		Canvas->DecBattleActionCursor();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->IncBattleActionCursor();
	}
	else if (UEngineInput::IsDown('X'))
	{
		State = EState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetBattleActionBoxActive(false);
	}
	else if (UEngineInput::IsDown('Z'))
	{
		SelectBattleAction();
	}
}

void UPokemonUILevel::ProcessBattleShiftFailMessageShow()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EState::TargetSelectionWait;
		Canvas->SetBattleMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetBattleActionBoxActive(false);
	}
}

void UPokemonUILevel::ProcessSwitchSelectionWait()
{
	if (true == UEngineInput::IsDown('X'))
	{
		State = EState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		SelectSwitch();
		return;
	}

	int EntrySize = UPlayerData::GetPokemonEntrySize();
	if (true == UEngineInput::IsDown(VK_LEFT)
		&& true == IsTargetCursorOnEntry())
	{
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
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = 1;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::From);
		TargetCursor = UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::To);
		Canvas->RefreshAllTargets();
	}
}


void UPokemonUILevel::SelectTarget()
{
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		// ��� ��ư�� ������ ���
		UEventManager::FadeChangeLevel(PrevLevelName, false);
	}
	else if (false == BattleMode)
	{
		State = EState::ActionSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(false);
		Canvas->SetActionSelectionMsgBoxActive(true);
		Canvas->SetActionBoxActive(true);
		Canvas->SetActionCursor(0);
	}
	else
	{
		State = EState::BattleActionSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(false);
		Canvas->SetActionSelectionMsgBoxActive(true);
		Canvas->SetBattleActionBoxActive(true);
		Canvas->SetActionCursor(0);
	}
}

void UPokemonUILevel::SelectAction()
{
	switch (Canvas->GetActionCursor())
	{
	case 0:
		// PokemonSummaryUI ������ ��ȯ
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 1:
		// Switch ���·� ��ȯ
		State = EState::SwitchSelectionWait;
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetSwitchSelectionMsgBoxActive(true);
		Canvas->SetActionBoxActive(false);
		SwitchFromCursor = TargetCursor;
		Canvas->RefreshAllTargets();
		break;
	case 2:
		// Target ���·� ����
		State = EState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
		break;
	default:
		break;
	}
}

void UPokemonUILevel::SelectBattleAction()
{
	switch (Canvas->GetBattleActionCursor())
	{
	case 0:
	{
		// Send Out
		const UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
		if (SelectedPokemon == PlayerBattler->CurPokemon())
		{
			// ��Ʋ�� ���� �ִ� ���ϸ��� ���� ���
			State = EState::BattleShiftFailMessageShow;
			Canvas->SetBattleMsgBoxActive(true);
			Canvas->SetActionBoxActive(false);
			Canvas->SetBattleMessage(SelectedPokemon->GetNameW() + +L" is already\nin battle!");
			return;
		}
		else if (true == SelectedPokemon->IsFaint())
		{
			// ������ ���ϸ��� ���� ���
			State = EState::BattleShiftFailMessageShow;
			Canvas->SetBattleMsgBoxActive(true);
			Canvas->SetActionBoxActive(false);
			Canvas->SetBattleMessage(SelectedPokemon->GetNameW() + L" has no energy\nleft to battle!");
			return;
		}

		UEventManager::FadeChangeLevel(PrevLevelName, false);
		PlayerBattler->SetShiftPokemonIndex(TargetCursor);
	}
	break;
	case 1:
		// PokemonSummaryUI ������ ��ȯ
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 2:
		// Target ���·� ����
		State = EState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetBattleActionBoxActive(false);
		break;
	default:
		break;
	}
}

void UPokemonUILevel::SelectSwitch()
{
	if (TargetCursor == SwitchFromCursor
		|| true == IsTargetCursorOnCancel())
	{
		// ����ġ ���
		State = EState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	State = EState::SwitchMoveOut;
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

void UPokemonUILevel::ProcessSwitchMoveOut()
{
	float t = Timer / SwitchMoveOutTime;
	SwitchFromBox->SetRelativePosition(UPokemonMath::Lerp(SwitchFromOutPos, SwitchFromInPos, t));
	SwitchToBox->SetRelativePosition(UPokemonMath::Lerp(SwitchToOutPos, SwitchToInPos, t));

	if (Timer <= 0.0f)
	{
		State = EState::SwitchMoveWait;
		Timer = SwitchMoveWaitTime;

		UPlayerData::SwapEntry(SwitchFromCursor, TargetCursor);
		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::To);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::From);

		Canvas->RefreshAllTargets(true);
	}
}

void UPokemonUILevel::ProcessSwitchMoveWait()
{
	if (Timer <= 0.0f)
	{
		State = EState::SwitchMoveIn;
		Timer = SwitchMoveInTime;
	}
}

void UPokemonUILevel::ProcessSwitchMoveIn()
{
	float t = Timer / SwitchMoveInTime;
	SwitchFromBox->SetRelativePosition(UPokemonMath::Lerp(SwitchFromInPos, SwitchFromOutPos, t));
	SwitchToBox->SetRelativePosition(UPokemonMath::Lerp(SwitchToInPos, SwitchToOutPos, t));

	if (Timer <= 0.0f)
	{
		State = EState::TargetSelectionWait;
		Canvas->SetSwitchSelectionMsgBoxActive(false);
		Canvas->SetTargetSelectionMsgBoxActive(true);

		Canvas->SetBoxState(SwitchFromCursor, APokemonCanvas::EBoxState::Unfocused);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);

		Canvas->RefreshAllTargets();
	}
}