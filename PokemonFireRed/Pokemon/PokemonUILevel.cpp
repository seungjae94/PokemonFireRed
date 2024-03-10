#include "PokemonUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonCanvas.h"
#include "MapLevel.h"
#include "BattleLevel.h"
#include "PokemonSummaryUILevel.h"
#include "BagUILevel.h"
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
	case EState::BattleShiftActionSelectionWait:
		ProcessBattleShiftActionSelectionWait();
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
	case EState::BagTestItemUseEffect:
		ProcessBagTestItemUseEffect();
		break;
	case EState::BagHpUpEffect:
		ProcessBagHpUpEffect();
		break;
	case EState::ReturnWait:
		ProcessReturnWait();
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
	UBagUILevel* BagUILevel = dynamic_cast<UBagUILevel*>(_PrevLevel);

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
		Mode = EMode::Pokemon;
		Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBox);
	}
	else if (nullptr != BagUILevel)
	{
		if (true == BagUILevel->IsBattleMode())
		{
			Mode = EMode::BattleItem;
		}
		else
		{
			Mode = EMode::Bag;
		}
		UseItem = BagUILevel->GetTargetItem();
		Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBoxBagMode);
	}
	// ��Ʋ ���� ó��
	else if (nullptr != BattleLevel)
	{
		Mode = EMode::BattleShift;
		PlayerBattler = BattleLevel->GetPlayerBattler();
		PlayerBattler->SetShiftPokemonIndex(PlayerBattler->CurPokemonIndex());
		Canvas->SetTargetSelectionMsgBoxImage(RN::PokemonUITargetSelectionMsgBox);
	}
}

void UPokemonUILevel::ProcessTargetSelectionWait()
{
	int EntrySize = UPlayerData::GetPokemonEntrySize();

	if (true == UEngineInput::IsDown('X'))
	{
		CancelTargetSelection();
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

void UPokemonUILevel::ProcessBattleShiftActionSelectionWait()
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
		Canvas->SetCustomMsgBoxActive(false);
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
	// ��� ��ư�� ������ ���
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		CancelTargetSelection();
	}
	// �޴�â�� ���� ���ϸ� UI ������ ���� ���
	else if (EMode::Pokemon == Mode)
	{
		State = EState::ActionSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(false);
		Canvas->SetActionSelectionMsgBoxActive(true);
		Canvas->SetActionBoxActive(true);
		Canvas->SetActionCursor(0);
	}
	// ��Ʋ �������� Pokemon �׼��� �����ؼ� ���ϸ� UI ������ ���� ���
	else if (EMode::BattleShift == Mode)
	{
		State = EState::BattleShiftActionSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(false);
		Canvas->SetActionSelectionMsgBoxActive(true);
		Canvas->SetBattleActionBoxActive(true);
		Canvas->SetActionCursor(0);
	}
	// ��Ʋ �������� Item �׼��� �����ؼ� ���� ������ ������ �� �ٽ� ������ ��� ����� ���� ���� ���ϸ� UI ������ ���� ���
	else if (EMode::BattleItem == Mode)
	{
		
	}
	// ���� �������� ������ ��� ����� ���� ���� ���ϸ� UI ������ ���� ���
	else if (EMode::Bag == Mode)
	{
		State = EState::BagTestItemUseEffect;
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
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetBattleActionBoxActive(false);
			Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + +L" is already\nin battle!");
			return;
		}
		else if (true == SelectedPokemon->IsFaint())
		{
			// ������ ���ϸ��� ���� ���
			State = EState::BattleShiftFailMessageShow;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetBattleActionBoxActive(false);
			Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" has no energy\nleft to battle!");
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

void UPokemonUILevel::ProcessBagTestItemUseEffect()
{
	UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
	EUseEffect UseEffect = UseItem->UseEffect;

	if (EUseEffect::Hp == UseEffect)
	{
		// Ǯ���� ���
		int CurHp = SelectedPokemon->GetCurHp();
		int MaxHp = SelectedPokemon->GetHp();

		if ( CurHp == MaxHp )
		{
			State = EState::ReturnWait;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");
			return;
		}

		PrevHealHp = CurHp;
		NextHealHp = UPokemonMath::Min(MaxHp, CurHp + UseItem->HealValue);
		SelectedPokemon->SetCurHp(NextHealHp);

		State = EState::BagHpUpEffect;
		Timer = HealTime;
	}
	else if (EUseEffect::CureAll == UseEffect)
	{
		// ���� ���� �Ǵ� ���� ������ ���
		EPokemonStatus StatusId = SelectedPokemon->GetStatusId();
		if (EPokemonStatus::Normal == StatusId || EPokemonStatus::Faint == StatusId)
		{
			State = EState::ReturnWait;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");
			return;
		}

		State = EState::ReturnWait;
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
			State = EState::ReturnWait;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");
			return;
		}

		State = EState::ReturnWait;
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
			State = EState::ReturnWait;
			Canvas->SetCustomMsgBoxActive(true);
			Canvas->SetCustomMessage(L"It won't have any effect.");
			return;
		}

		State = EState::ReturnWait;
		SelectedPokemon->SetStatus(EPokemonStatus::Normal);
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L" was cured of it's poisoning.");
		Canvas->RefreshAllTargets();
	}
}

void UPokemonUILevel::ProcessBagHpUpEffect()
{
	const UPokemon* SelectedPokemon = &UPlayerData::GetPokemonInEntry(TargetCursor);
	Canvas->LerpHeal(TargetCursor, PrevHealHp, NextHealHp, SelectedPokemon->GetHp(), Timer / HealTime);

	if (Timer <= 0.0f)
	{
		State = EState::ReturnWait;

		
		Canvas->SetCustomMsgBoxActive(true);
		Canvas->SetCustomMessage(SelectedPokemon->GetNameW() + L"'s HP was restored\nby " + std::to_wstring(NextHealHp - PrevHealHp) + L" points.");
	}
}

void UPokemonUILevel::ProcessReturnWait()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		UEventManager::FadeChangeLevel(PrevLevelName);
	}
}

void UPokemonUILevel::CancelTargetSelection()
{
	// ��Ʋ �� �÷��̾� ���ϸ��� �����ؼ� ������ ���ϸ� ������ �� ��� -> Cancel ���� �Ұ���
	if (EMode::BattleShift == Mode && PlayerBattler->CurPokemon()->IsFaint())
	{
		State = EState::TargetSelectionWait;
		return;
	}

	UEventManager::FadeChangeLevel(PrevLevelName, false);
	Canvas->SetActionCursor(0);
}