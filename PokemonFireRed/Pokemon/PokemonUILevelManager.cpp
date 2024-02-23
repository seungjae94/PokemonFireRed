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

	// 타겟 선택 메시지 박스
	{
		LongMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		LongMsgBoxRenderer->SetImage("UPLongMsgBox.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(LongMsgBoxRenderer);
		FVector Pos = UPokemonUtil::GetLeftBotAlignPos(RenderScale);
		Pos += UPokemonUtil::PixelVector(2, -2);
		LongMsgBoxRenderer->SetTransform({ Pos, RenderScale });
	}

	// 액션 선택 메시지 박스
	{
		ShortMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		ShortMsgBoxRenderer->SetImage("UPShortMsgBox.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(ShortMsgBoxRenderer);
		FVector Pos = UPokemonUtil::GetLeftBotAlignPos(RenderScale);
		Pos += UPokemonUtil::PixelVector(2, -2);
		ShortMsgBoxRenderer->SetTransform({ Pos, RenderScale });
		ShortMsgBoxRenderer->SetActive(false);
	}

	// 액션 선택 창
	{
		ActionBoxRenderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		ActionBoxRenderer->SetImage("UPActionBox.png");
		FVector RenderScale = UPokemonUtil::GetRenderScale(ActionBoxRenderer);
		FVector Pos = UPokemonUtil::GetRightBotAlignPos(RenderScale);
		Pos += UPokemonUtil::PixelVector(-1, -1);
		ActionBoxRenderer->SetTransform({ Pos, RenderScale });
		ActionBoxRenderer->SetActive(false);
	}

	// 엔트리 박스
	FirstRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	FirstMiniPokemonRenderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
	UPokemonUtil::CreateMiniPokemonAnimations(FirstMiniPokemonRenderer);
	DrawFirst(ETargetImageState::Focused);

	EntryRenderers.reserve(6);
	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		UImageRenderer* MiniPokemonRenderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		UPokemonUtil::CreateMiniPokemonAnimations(MiniPokemonRenderer);
		EntryRenderers.push_back(Renderer);
		EntryMiniPokemonRenderers.push_back(MiniPokemonRenderer);
		DrawEntry(ETargetImageState::Unfocused, i);
	}
	for (int i = UPlayerData::GetPokemonEntrySize(); i < 6; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		UImageRenderer* MiniPokemonRenderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		UPokemonUtil::CreateMiniPokemonAnimations(MiniPokemonRenderer);
		EntryRenderers.push_back(Renderer);
		EntryMiniPokemonRenderers.push_back(MiniPokemonRenderer);
		DrawEntry(ETargetImageState::Empty, i);
	}

	CancelRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	DrawCancel(ETargetImageState::Unfocused);

	// 첫 번째 포켓몬 텍스트
	UPokemon& First = UPlayerData::GetPokemonInEntry(0);
	UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
	FirstNameText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot);
	FirstLevelText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot);
	FirstHpText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot, EAlignType::Right);
	FirstCurHpText = CurLevel->SpawnText(FirstRenderer, EPivotType::RightBot, EAlignType::Right);

	FirstNameText->SetSize(EFontSize::Mini);
	FirstLevelText->SetSize(EFontSize::Mini);
	FirstHpText->SetSize(EFontSize::Mini);
	FirstCurHpText->SetSize(EFontSize::Mini);

	FirstNameText->SetColor(EFontColor::WhiteGray);
	FirstLevelText->SetColor(EFontColor::WhiteGray);
	FirstHpText->SetColor(EFontColor::WhiteGray);
	FirstCurHpText->SetColor(EFontColor::WhiteGray);

	FirstNameText->SetText(First.GetName(), true);
	FirstLevelText->SetText(std::to_wstring(First.GetLevel()), true);
	FirstHpText->SetText(std::to_wstring(First.GetHp()), true);
	FirstCurHpText->SetText(std::to_wstring(First.GetCurHp()), true);

	FirstNameText->SetRelativePos(UPokemonUtil::PixelVector(-53, -28));
	FirstLevelText->SetRelativePos(UPokemonUtil::PixelVector(-37, -19));
	FirstHpText->SetRelativePos(UPokemonUtil::PixelVector(-5, -3));
	FirstCurHpText->SetRelativePos(UPokemonUtil::PixelVector(-25, -3));

	// 첫 번째 포켓몬 스크롤 바
	FirstHpBar = CurLevel->SpawnScrollBar(FirstRenderer, EPivotType::RightBot);
	FirstHpBar->SetRelativePos(UPokemonUtil::PixelVector(-53, -15));
	FirstHpBar->SetMaxValue(First.GetHp());
	FirstHpBar->SetValue(First.GetCurHp());

	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		APokemonText* NameText = CurLevel->SpawnText(EntryRenderers[i - 1], EPivotType::RightBot);
		APokemonText* LevelText = CurLevel->SpawnText(EntryRenderers[i - 1], EPivotType::LeftBot);
		APokemonText* HpText = CurLevel->SpawnText(EntryRenderers[i - 1], EPivotType::RightBot, EAlignType::Right);
		APokemonText* CurHpText = CurLevel->SpawnText(EntryRenderers[i - 1], EPivotType::RightBot, EAlignType::Right);

		NameText->SetSize(EFontSize::Mini);
		LevelText->SetSize(EFontSize::Mini);
		HpText->SetSize(EFontSize::Mini);
		CurHpText->SetSize(EFontSize::Mini);

		NameText->SetColor(EFontColor::WhiteGray);
		LevelText->SetColor(EFontColor::WhiteGray);
		HpText->SetColor(EFontColor::WhiteGray);
		CurHpText->SetColor(EFontColor::WhiteGray);

		UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(i);
		NameText->SetText(Pokemon.GetName(), true);
		LevelText->SetText(std::to_wstring(Pokemon.GetLevel()), true);
		HpText->SetText(std::to_wstring(Pokemon.GetHp()), true);
		CurHpText->SetText(std::to_wstring(Pokemon.GetCurHp()), true);

		NameText->SetRelativePos(UPokemonUtil::PixelVector(-119, -9));
		LevelText->SetRelativePos(UPokemonUtil::PixelVector(48, 0));
		HpText->SetRelativePos(UPokemonUtil::PixelVector(-5, 0));
		CurHpText->SetRelativePos(UPokemonUtil::PixelVector(-25, 0));

		EntryNameTexts.push_back(NameText);
		EntryLevelTexts.push_back(LevelText);
		EntryHpTexts.push_back(HpText);
		EntryCurHpTexts.push_back(CurHpText);

		// i번째 포켓몬 스크롤 바
		AScrollBar* HpBar = CurLevel->SpawnScrollBar(EntryRenderers[i - 1], EPivotType::RightBot);
		HpBar->SetRelativePos(UPokemonUtil::PixelVector(-53, -13));
		HpBar->SetMaxValue(Pokemon.GetHp());
		HpBar->SetValue(Pokemon.GetCurHp());
		EntryHpBars.push_back(HpBar);
	}
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
		// 캔슬 버튼 입력
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(PrevMapLevelName);
	}
	else
	{
		State = EPokemonUIState::ActionSelectionWait;

		// UI 띄우기
		LongMsgBoxRenderer->SetActive(false);
		ShortMsgBoxRenderer->SetActive(true);
		ActionBoxRenderer->SetActive(true);
	}
}

void UPokemonUILevelManager::DrawFirst(ETargetImageState _State)
{
	std::string ImageName;
	FVector AddPos;
	FVector MiniAddPos;

	switch (_State)
	{
	case UPokemonUILevelManager::ETargetImageState::Unfocused:
		ImageName = "UPFirst.png";
		AddPos = UPokemonUtil::PixelVector(2, 20);
		MiniAddPos = UPokemonUtil::PixelVector(-1, 12);
		break;
	case UPokemonUILevelManager::ETargetImageState::Focused:
		ImageName = "UPFirstFocused.png";
		AddPos = UPokemonUtil::PixelVector(2, 18);
		MiniAddPos = UPokemonUtil::PixelVector(-1, 14);
		break;
	case UPokemonUILevelManager::ETargetImageState::From:
		ImageName = "UPFirstFrom.png";
		AddPos = UPokemonUtil::PixelVector(2, 20);
		MiniAddPos = UPokemonUtil::PixelVector(-1, 12);
		break;
	case UPokemonUILevelManager::ETargetImageState::To:
		ImageName = "UPFirstTo.png";
		AddPos = UPokemonUtil::PixelVector(2, 18);
		MiniAddPos = UPokemonUtil::PixelVector(-1, 14);
		break;
	default:
		MsgBoxAssert("Pokemon UI의 First의 상태가 Empty입니다.");
		break;
	}

	FirstRenderer->SetImage(ImageName);
	FVector RenderScale = UPokemonUtil::GetRenderScale(FirstRenderer);
	FVector Pos = RenderScale.Half2D() + AddPos;
	FirstRenderer->SetTransform({ Pos, RenderScale });

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(0);
	EPokedexNo PokedexNo = Pokemon.GetPokedexNo();
	FirstMiniPokemonRenderer->ChangeAnimation(Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(PokedexNo));
	FVector MiniRenderScale =  Global::MiniPokemonRenderScale;
	FVector MiniPos = FirstRenderer->GetTransform().LeftTop() + MiniAddPos + MiniRenderScale.Half2D();
	FirstMiniPokemonRenderer->SetTransform({ MiniPos, MiniRenderScale });
}

void UPokemonUILevelManager::DrawEntry(ETargetImageState _State, int _Index)
{
	std::string ImageName;
	FVector AddPos;
	FVector MiniAddPos;

	switch (_State)
	{
	case UPokemonUILevelManager::ETargetImageState::Empty:
		ImageName = "UPEntryEmpty.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		MiniAddPos = UPokemonUtil::PixelVector(-3, 0);
		break;
	case UPokemonUILevelManager::ETargetImageState::Unfocused:
		ImageName = "UPEntry.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		MiniAddPos = UPokemonUtil::PixelVector(-3, 0);
		break;
	case UPokemonUILevelManager::ETargetImageState::Focused:
		ImageName = "UPEntryFocused.png";
		AddPos = UPokemonUtil::PixelVector(-2, 9 + 24 * (_Index - 1));
		MiniAddPos = UPokemonUtil::PixelVector(-3, 1);
		break;
	case UPokemonUILevelManager::ETargetImageState::From:
		ImageName = "UPEntryFrom.png";
		AddPos = UPokemonUtil::PixelVector(-2, 10 + 24 * (_Index - 1));
		MiniAddPos = UPokemonUtil::PixelVector(-3, 0);
		break;
	case UPokemonUILevelManager::ETargetImageState::To:
		ImageName = "UPEntryTo.png";
		AddPos = UPokemonUtil::PixelVector(-2, 9 + 24 * (_Index - 1));
		MiniAddPos = UPokemonUtil::PixelVector(-3, 1);
		break;
	default:
		break;
	}

	EntryRenderers[_Index - 1]->SetImage(ImageName);
	FVector RenderScale = UPokemonUtil::GetRenderScale(EntryRenderers[_Index - 1]);
	FVector Pos = UPokemonUtil::GetRightTopAlignPos(RenderScale) + AddPos;
	EntryRenderers[_Index - 1]->SetTransform({ Pos, RenderScale });

	if (_Index < 0 || _Index >= UPlayerData::GetPokemonEntrySize())
	{
		return;
	}

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(_Index);
	EPokedexNo PokedexNo = Pokemon.GetPokedexNo();
	EntryMiniPokemonRenderers[_Index - 1]->ChangeAnimation(Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(PokedexNo));

	FVector MiniRenderScale = Global::MiniPokemonRenderScale;
	FVector MiniPos = EntryRenderers[_Index - 1]->GetTransform().LeftTop() + MiniAddPos + MiniRenderScale.Half2D();
	EntryMiniPokemonRenderers[_Index - 1]->SetTransform({ MiniPos, MiniRenderScale });
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
