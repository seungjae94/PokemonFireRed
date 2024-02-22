#include "PokemonUILevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"
#include "PokemonUILevel.h"
#include "Pokemon.h"

UPokemonUILevelManager::UPokemonUILevelManager()
{
}

UPokemonUILevelManager::~UPokemonUILevelManager()
{
}

void UPokemonUILevelManager::BeginPlay()
{
	// 백그라운드
	BackgroundRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	BackgroundRenderer->SetImage("UPBackground.png");
	FVector BackgroundRenderScale = UPokemonUtil::GetRenderScale(BackgroundRenderer);
	FVector BackgroundPos = BackgroundRenderScale.Half2D();
	BackgroundRenderer->SetTransform({ BackgroundPos, BackgroundRenderScale });

	// 메시지 박스
	{
		LongMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		LongMsgBoxRenderer->SetImage("UPLongMsgBox.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(LongMsgBoxRenderer);
		FVector Pos = UPokemonUtil::GetLeftBotAlignPos(RenderScale);
		Pos += UPokemonUtil::PixelVector(2, -2);
		LongMsgBoxRenderer->SetTransform({ Pos, RenderScale });
	}
	{
		ShortMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		ShortMsgBoxRenderer->SetImage("UPShortMsgBox.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(ShortMsgBoxRenderer);
		FVector Pos = UPokemonUtil::GetLeftBotAlignPos(RenderScale);
		Pos += UPokemonUtil::PixelVector(2, -2);
		ShortMsgBoxRenderer->SetTransform({ Pos, RenderScale });
		ShortMsgBoxRenderer->SetActive(false);
	}

	// 엔트리
	FirstRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	DrawFirst(ETargetImageState::Focused);

	EntryRenderers.reserve(6);
	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		EntryRenderers.push_back(Renderer);
		DrawEntry(ETargetImageState::Unfocused, i);
	}
	for (int i = UPlayerData::GetPokemonEntrySize(); i < 6; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		EntryRenderers.push_back(Renderer);
		DrawEntry(ETargetImageState::Empty, i);
	}

	CancelRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	DrawCancel(ETargetImageState::Unfocused);

	// 텍스트 렌더링
	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
	FirstNameText = CurLevel->SpawnText(FirstRenderer, EPivotType::LeftTop);
	FirstLevelText = CurLevel->SpawnText(FirstRenderer, EPivotType::LeftTop);
	FirstHpText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot, EAlignType::Right);
	FirstCurHpText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot, EAlignType::Right);

	FirstNameText->SetSize(EFontSize::Mini);
	FirstLevelText->SetSize(EFontSize::Mini);
	FirstHpText->SetSize(EFontSize::Mini);
	FirstCurHpText->SetSize(EFontSize::Mini);

	UPokemon& First = UPlayerData::GetPokemonInEntry(0);
	FirstNameText->SetText(First.GetName(), true);
	FirstLevelText->SetText(std::to_wstring(First.GetLevel()), true);
	FirstHpText->SetText(std::to_wstring(First.GetHp()), true);
	FirstCurHpText->SetText(std::to_wstring(First.GetCurHp()), true);

	FirstNameText->SetRelativePos(UPokemonUtil::PixelVector(25, 26));
	FirstLevelText->SetRelativePos(UPokemonUtil::PixelVector(48, 37));
	FirstHpText->SetRelativePos(UPokemonUtil::PixelVector(-6, -4));
	FirstCurHpText->SetRelativePos(UPokemonUtil::PixelVector(-26, -4));
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
	LongMsgBoxRenderer->SetActive(true);
	ShortMsgBoxRenderer->SetActive(false);

	if (true == UEngineInput::IsDown('X'))
	{
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(PrevMapLevelName);
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		TargetSelect();
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
		DrawFirst(ETargetImageState::Unfocused);
	}
	else if (true == IsCancel(TargetCursor))
	{
		DrawCancel(ETargetImageState::Unfocused);
	}
	else if (true == IsEntry(TargetCursor))
	{
		DrawEntry(ETargetImageState::Unfocused, TargetCursor);
	}

	TargetCursor = _Cursor;

	// 다음 커서 위치의 이미지 변경
	if (true == IsFirst(TargetCursor))
	{
		DrawFirst(ETargetImageState::Focused);
	}
	else if (true == IsCancel(TargetCursor))
	{
		DrawCancel(ETargetImageState::Focused);
	}
	else if (true == IsEntry(TargetCursor))
	{
		DrawEntry(ETargetImageState::Focused, TargetCursor);
	}

}

void UPokemonUILevelManager::TargetSelect()
{
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(PrevMapLevelName);
	}
	else
	{
		State = EPokemonUIState::ActionSelectionWait;
		// UI 띄우기
	}
}

void UPokemonUILevelManager::DrawFirst(ETargetImageState _State)
{
	std::string ImageName;
	FVector AddPos;

	switch (_State)
	{
	case UPokemonUILevelManager::ETargetImageState::Unfocused:
		ImageName = "UPFirst.png";
		AddPos = UPokemonUtil::PixelVector(2, 20);
		break;
	case UPokemonUILevelManager::ETargetImageState::Focused:
		ImageName = "UPFirstFocused.png";
		AddPos = UPokemonUtil::PixelVector(2, 18);
		break;
	case UPokemonUILevelManager::ETargetImageState::From:
		ImageName = "UPFirstFrom.png";
		AddPos = UPokemonUtil::PixelVector(2, 20);
		break;
	case UPokemonUILevelManager::ETargetImageState::To:
		ImageName = "UPFirstTo.png";
		AddPos = UPokemonUtil::PixelVector(2, 18);
		break;
	default:
		MsgBoxAssert("Pokemon UI의 First의 상태가 Empty입니다.");
		break;
	}

	FirstRenderer->SetImage(ImageName);
	FVector FirstRenderScale = UPokemonUtil::GetRenderScale(FirstRenderer);
	FVector FirstPos = FirstRenderScale.Half2D();
	FirstPos += AddPos;
	FirstRenderer->SetTransform({ FirstPos, FirstRenderScale });
}

void UPokemonUILevelManager::DrawEntry(ETargetImageState _State, int _Index)
{
	std::string ImageName;
	FVector AddPos;

	switch (_State)
	{
	case UPokemonUILevelManager::ETargetImageState::Empty:
		ImageName = "UPEntryEmpty.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		break;
	case UPokemonUILevelManager::ETargetImageState::Unfocused:
		ImageName = "UPEntry.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		break;
	case UPokemonUILevelManager::ETargetImageState::Focused:
		ImageName = "UPEntryFocused.png";
		AddPos = UPokemonUtil::PixelVector(-2, 9 + 24 * (_Index - 1));
		break;
	case UPokemonUILevelManager::ETargetImageState::From:
		ImageName = "UPEntryFrom.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		break;
	case UPokemonUILevelManager::ETargetImageState::To:
		ImageName = "UPEntryTo.png";
		AddPos = UPokemonUtil::PixelVector(-2, 9 + 24 * (_Index - 1));
		break;
	default:
		break;
	}

	EntryRenderers[_Index - 1]->SetImage(ImageName);
	FVector RenderScale = UPokemonUtil::GetRenderScale(EntryRenderers[_Index - 1]);
	FVector Pos = UPokemonUtil::GetRightTopAlignPos(RenderScale);
	Pos += AddPos;
	EntryRenderers[_Index - 1]->SetTransform({Pos, RenderScale});
}

void UPokemonUILevelManager::DrawCancel(ETargetImageState _State)
{
	std::string ImageName;
	FVector AddPos;

	switch (_State)
	{
	case UPokemonUILevelManager::ETargetImageState::Unfocused:
		ImageName = "UPCancel.png";
		AddPos = UPokemonUtil::PixelVector(-2, -6);
		break;
	case UPokemonUILevelManager::ETargetImageState::Focused:
		ImageName = "UPCancelFocused.png";
		AddPos = UPokemonUtil::PixelVector(-2, -4);
		break;
	default:
		MsgBoxAssert("Pokemon UI에서 Cancel의 상태가 From 또는 To입니다.");
		break;
	}

	CancelRenderer->SetImage(ImageName);
	FVector CancelRenderScale = UPokemonUtil::GetRenderScale(CancelRenderer);
	FVector CancelPos = UPokemonUtil::GetRightBotAlignPos(CancelRenderScale);
	CancelPos += AddPos;
	CancelRenderer->SetTransform({ CancelPos, CancelRenderScale });
}

void UPokemonUILevelManager::ActionSelectionWaitTick(float _DeltaTime)
{
}

void UPokemonUILevelManager::SwitchTick(float _DeltaTime)
{
}
