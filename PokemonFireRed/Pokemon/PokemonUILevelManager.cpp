#include "PokemonUILevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"

UPokemonUILevelManager::UPokemonUILevelManager()
{
}

UPokemonUILevelManager::~UPokemonUILevelManager()
{
}

void UPokemonUILevelManager::BeginPlay()
{
	BackgroundRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	BackgroundRenderer->SetImage("UPBackground.png");
	FVector BackgroundRenderScale = UPokemonUtil::GetRenderScale(BackgroundRenderer);
	FVector BackgroundPos = BackgroundRenderScale.Half2D();
	BackgroundRenderer->SetTransform({ BackgroundPos, BackgroundRenderScale });

	FirstRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	FirstRenderer->SetImage("UPFirstFocused.png");
	FVector FirstRenderScale = UPokemonUtil::GetRenderScale(FirstRenderer);
	FVector FirstPos = FirstRenderScale.Half2D();
	FirstPos += FVector(2.0f, 20.0f) * Global::FloatPixelSize;
	FirstRenderer->SetTransform({ FirstPos, FirstRenderScale });

	EntryRenderers.reserve(6);
	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		Renderer->SetImage("UPEntry.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
		FVector Pos = UPokemonUtil::GetRightTopAlignPos(RenderScale);
		Pos += FVector(-3.0f, 11.0f + 24.0f * (i - 1)) * Global::FloatPixelSize;
		Renderer->SetTransform({ Pos, RenderScale });
		EntryRenderers.push_back(Renderer);
	}
	for (int i = UPlayerData::GetPokemonEntrySize(); i < 6; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		Renderer->SetImage("UPEntryEmpty.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(Renderer);
		FVector Pos = UPokemonUtil::GetRightTopAlignPos(RenderScale); 
		Pos += FVector(-3.0f, 11.0f + 24.0f * (i - 1)) * Global::FloatPixelSize;
		Renderer->SetTransform({ Pos, RenderScale });
		EntryRenderers.push_back(Renderer);
	}

	CancelRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	CancelRenderer->SetImage("UPCancel.png");
	FVector CancelRenderScale = UPokemonUtil::GetRenderScale(CancelRenderer);
	FVector CancelPos = UPokemonUtil::GetRightBotAlignPos(CancelRenderScale);
	CancelPos += FVector(-3.0f, -7.0f) * Global::FloatPixelSize;
	CancelRenderer->SetTransform({ CancelPos, CancelRenderScale });
}

void UPokemonUILevelManager::Tick(float _DeltaTime)
{
	switch (State)
	{
	case EPokemonUIState::TargetSelectionWait:
		TargetSelectionWaitTick(_DeltaTime);
		break;
	case EPokemonUIState::ActionSelectionWait:
		ActionSelectionWaitTick(_DeltaTime);
		break;
	case EPokemonUIState::Switch:
		SwitchTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void UPokemonUILevelManager::TargetSelectionWaitTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::SetLevel(PrevMapLevelName);
		return;
	}

	int EntrySize = UPlayerData::GetPokemonEntrySize();
	if (true == UEngineInput::IsDown(VK_LEFT) && IsEntry(TargetCursor))
	{
		MemoryEntryCursor = TargetCursor;
		MoveTargetCursor(0);
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT) && IsFirst(TargetCursor))
	{
		MoveTargetCursor(MemoryEntryCursor);
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		if (true == IsEntry(TargetCursor))
		{
			MemoryEntryCursor = TargetCursor;
		}

		MoveTargetCursor(UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1));
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (true == IsEntry(TargetCursor))
		{
			MemoryEntryCursor = TargetCursor;
		}

		MoveTargetCursor(UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1));
		return;
	}
}

void UPokemonUILevelManager::MoveTargetCursor(int _Cursor)
{
	// 이전 커서 위치의 이미지 변경
	if (true == IsFirst(TargetCursor))
	{
		FirstRenderer->SetImage("UPFirst.png");
	}
	else if (true == IsCancel(TargetCursor))
	{
		CancelRenderer->SetImage("UPCancel.png");
	}
	else if (true == IsEntry(TargetCursor))
	{
		EntryRenderers[TargetCursor - 1]->SetImage("UPEntry.png");
	}

	TargetCursor = _Cursor;

	// 다음 커서 위치의 이미지 변경
	if (true == IsFirst(TargetCursor))
	{
		FirstRenderer->SetImage("UPFirstFocused.png");
	}
	else if (true == IsCancel(TargetCursor))
	{
		CancelRenderer->SetImage("UPCancelFocused.png");
	}
	else if (true == IsEntry(TargetCursor))
	{
		EntryRenderers[TargetCursor - 1]->SetImage("UPEntryFocused.png");
	}

}

void UPokemonUILevelManager::ActionSelectionWaitTick(float _DeltaTime)
{
}

void UPokemonUILevelManager::SwitchTick(float _DeltaTime)
{
}
