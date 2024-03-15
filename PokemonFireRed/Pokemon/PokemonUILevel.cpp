#include "PokemonUILevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "SoundManager.h"
#include "PokemonCanvas.h"
#include "MapLevel.h"
#include "BattleLevel.h"
#include "PokemonSummaryUILevel.h"
#include "BagUILevel.h"
#include "BattleUtil.h"

const EPokemonUIState EPokemonUIState::None;
const EPokemonUIState EPokemonUIState::TargetSelectionWait;
const EPokemonUIState EPokemonUIState::ActionSelectionWait;
int EPokemonUIState::MaxIndex = 0;

void EPokemonUIState::operator=(const EPokemonUIState& _Other)
{
	Index = _Other.Index;
}

bool EPokemonUIState::operator==(const EPokemonUIState& _Other)
{
	return Index == _Other.Index;
}

bool EPokemonUIState::operator<(const EPokemonUIState& _Other)
{
	return Index < _Other.Index;
}

bool EPokemonUIState::operator>(const EPokemonUIState& _Other)
{
	return Index > _Other.Index;
}

bool UPokemonUILevel::IsCommonResourcesLoaded = false;

UPokemonUILevel::UPokemonUILevel()
{
}

UPokemonUILevel::~UPokemonUILevel()
{
}

void UPokemonUILevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	if (false == IsCommonResourcesLoaded)
	{
		UEngineDirectory CurDir;
		CurDir.MoveToSearchChild("Resources");
		CurDir.Move("PokemonUILevel");

		std::list<UEngineFile> Files = CurDir.AllFile({ ".bmp", ".png" }, true);
		for (UEngineFile& File : Files)
		{
			UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
		}

		IsCommonResourcesLoaded = true;
	}

	Canvas = SpawnActor<APokemonCanvas>();
}

void UPokemonUILevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);

	// 이전 레벨이 SummaryUI 레벨인 경우 레벨 초기화를 하지 않는다.
	UPokemonSummaryUILevel* SummaryUILevel = dynamic_cast<UPokemonSummaryUILevel*>(_PrevLevel);

	if (nullptr != SummaryUILevel)
	{
		return;
	}

	// 레벨 초기화
	Canvas->Init();
	TargetCursor = 0;
}

void UPokemonUILevel::ProcessTargetSelectionWait()
{
	int EntrySize = UPlayerData::GetPokemonEntrySize();

	// 타겟 선택 단계에서 취소 버튼을 누를 때의 행동
	if (true == UEngineInput::IsDown('X'))
	{
		PlaySEClick();
		CancelTargetSelection();
		return;
	}

	// 타겟 선택 단계에서 타겟을 선택할 때의 행동
	if (true == UEngineInput::IsDown('Z'))
	{
		PlaySEClick();
		SelectTarget();
		return;
	}

	// 이하 커서 이동 처리

	if (true == UEngineInput::IsDown(VK_LEFT)
		&& true == IsTargetCursorOnEntry())
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = 0;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_RIGHT)
		&& true == IsTargetCursorOnFirst()
		&& EntrySize > 1)
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = 1;
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		PlaySEClick();
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Unfocused);
		TargetCursor = UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1);
		Canvas->SetBoxState(TargetCursor, APokemonCanvas::EBoxState::Focused);
		Canvas->RefreshAllTargets();
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		PlaySEClick();
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
		PlaySEClick();
		Canvas->DecActionCursor();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		PlaySEClick();
		Canvas->IncActionCursor();
	}
	else if (UEngineInput::IsDown('X'))
	{
		PlaySEClick();
		State = EPokemonUIState::TargetSelectionWait;
		Canvas->SetTargetSelectionMsgBoxActive(true);
		Canvas->SetActionSelectionMsgBoxActive(false);
		Canvas->SetActionBoxActive(false);
	}
	else if (UEngineInput::IsDown('Z'))
	{
		PlaySEClick();
		SelectAction();
	}
}

bool UPokemonUILevel::IsTargetCursorOnFirst() const
{
	return TargetCursor == 0;
}

bool UPokemonUILevel::IsTargetCursorOnEntry() const
{
	return TargetCursor > 0 && TargetCursor < UPlayerData::GetPokemonEntrySize();
}

bool UPokemonUILevel::IsTargetCursorOnCancel() const
{
	return TargetCursor == UPlayerData::GetPokemonEntrySize();
}

void UPokemonUILevel::PlaySEClick()
{
	USoundManager::PlaySE(RN::SEClick);
}
