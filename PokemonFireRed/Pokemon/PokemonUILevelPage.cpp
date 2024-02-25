#include "PokemonUILevelPage.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"
#include "PokemonUILevel.h"
#include "Pokemon.h"

UPokemonUILevelPage::UPokemonUILevelPage()
{
}

UPokemonUILevelPage::~UPokemonUILevelPage()
{
}

void UPokemonUILevelPage::BeginPlay()
{
	// 백그라운드
	BackgroundRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	BackgroundRenderer->SetImage(RN::PokemonUIBackground);
	FVector BackgroundRenderScale = UPokemonUtil::GetRenderScale(BackgroundRenderer);
	FVector BackgroundPos = BackgroundRenderScale.Half2D();
	BackgroundRenderer->SetTransform({ BackgroundPos, BackgroundRenderScale });

	// 타겟 선택 메시지 박스
	{
		TargetSelectionMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		TargetSelectionMsgBoxRenderer->SetImage(RN::PokemonUITargetSelectionMsgBox);
		UPokemonUtil::PlaceImageOnScreen(
			TargetSelectionMsgBoxRenderer,
			EPivotType::LeftBot,
			2, -2
		);
	}

	// 액션 선택 메시지 박스
	{
		ActionSelectionMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		ActionSelectionMsgBoxRenderer->SetImage(RN::PokemonUIActionSelectionMsgBox);
		ActionSelectionMsgBoxRenderer->SetActive(false);
		UPokemonUtil::PlaceImageOnScreen(
			ActionSelectionMsgBoxRenderer,
			EPivotType::LeftBot,
			2, -2
		);
	}

	// 액션 선택 창
	{
		ActionBoxRenderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		ActionBoxRenderer->SetImage(RN::PokemonUIActionBox);
		UPokemonUtil::PlaceImageOnScreen(
			ActionBoxRenderer,
			EPivotType::RightBot,
			-1, -1
		);

		// 액션 선택 커서
		ActionCursor = CreateCursor(
			ActionBoxRenderer,
			0, 3,
			EPivotType::LeftTop,
			8, 11, 16
		);

		ActionBoxRenderer->SetActive(false);
	}

	// 스위치 선택 메시지 박스
	{
		SwitchSelectionMsgBoxRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		SwitchSelectionMsgBoxRenderer->SetImage(RN::PokemonUISwitchSelectionMsgBox);
		UPokemonUtil::PlaceImageOnScreen(
			SwitchSelectionMsgBoxRenderer,
			EPivotType::LeftBot,
			2, -2
		);
		SwitchSelectionMsgBoxRenderer->SetActive(false);
	}

	// 엔트리 박스
	FirstRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);

	EntryRenderers.reserve(6);
	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		EntryRenderers.push_back(Renderer);
	}
	for (int i = UPlayerData::GetPokemonEntrySize(); i < 6; ++i)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		EntryRenderers.push_back(Renderer);
	}

	CancelRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);

	// 텍스트, 아이콘, 스크롤바
	UPokemon& First = UPlayerData::GetPokemonInEntry(0);
	FirstNameText = CreateText(
		FirstRenderer,
		First.GetName(),
		EPivotType::RightBot,
		EAlignType::Left,
		-53, -28,
		EFontColor::WhiteGray, EFontSize::Mini
	);
	FirstLevelText = CreateText(
		FirstRenderer,
		std::to_wstring(First.GetLevel()),
		EPivotType::RightBot,
		EAlignType::Left,
		-37, -19,
		EFontColor::WhiteGray, EFontSize::Mini
	);
	FirstHpText = CreateText(
		FirstRenderer,
		std::to_wstring(First.GetHp()),
		EPivotType::RightBot,
		EAlignType::Right,
		-5, -3,
		EFontColor::WhiteGray, EFontSize::Mini
	);
	FirstCurHpText = CreateText(
		FirstRenderer,
		std::to_wstring(First.GetCurHp()),
		EPivotType::RightBot,
		EAlignType::Right,
		-25, -3,
		EFontColor::WhiteGray, EFontSize::Mini
	);
	FirstHpBar = CreateScrollBar(
		FirstRenderer,
		EScrollType::Hp,
		First.GetCurHp(), First.GetHp(),
		EPivotType::RightBot,
		-53, -15
	);
	FirstPokemonIcon = CreatePokemonElement(
		FirstRenderer,
		EPokemonElementType::Mini,
		EPivotType::LeftBot,
		-2, -23
	);
	FirstGender = CreateImageElement(
		FirstRenderer,
		EPivotType::RightBot,
		-9, -19
	);

	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(i);

		AText* NameText = CreateText(
			EntryRenderers[i - 1],
			Pokemon.GetName(),
			EPivotType::RightBot,
			EAlignType::Left,
			-119, -9,
			EFontColor::WhiteGray, EFontSize::Mini
		);
		AText* LevelText = CreateText(
			EntryRenderers[i - 1],
			std::to_wstring(Pokemon.GetLevel()),
			EPivotType::LeftBot,
			EAlignType::Left,
			48, 0,
			EFontColor::WhiteGray, EFontSize::Mini
		);
		AText* HpText = CreateText(
			EntryRenderers[i - 1],
			std::to_wstring(Pokemon.GetHp()),
			EPivotType::RightBot,
			EAlignType::Right,
			-5, 0,
			EFontColor::WhiteGray, EFontSize::Mini
		);
		AText* CurHpText = CreateText(
			EntryRenderers[i - 1],
			std::to_wstring(Pokemon.GetCurHp()),
			EPivotType::RightBot,
			EAlignType::Right,
			-25, 0,
			EFontColor::WhiteGray, EFontSize::Mini
		);
		AScrollBar* HpBar = CreateScrollBar(
			EntryRenderers[i - 1],
			EScrollType::Hp,
			Pokemon.GetCurHp(), Pokemon.GetHp(),
			EPivotType::RightBot,
			-53, -13
		);
		APokemonElement* Icon = CreatePokemonElement(
			EntryRenderers[i - 1],
			EPokemonElementType::Mini,
			EPivotType::LeftBot,
			-3, 3
		);
		AImageElement* Gender = CreateImageElement(
			EntryRenderers[i - 1],
			EPivotType::RightBot,
			-72, 0
		);

		EntryNameTexts.push_back(NameText);
		EntryLevelTexts.push_back(LevelText);
		EntryHpTexts.push_back(HpText);
		EntryCurHpTexts.push_back(CurHpText);
		EntryHpBars.push_back(HpBar);
		EntryPokemonIcons.push_back(Icon);
		EntryGenders.push_back(Gender);
	}

	RefreshAllTargets();
}

void UPokemonUILevelPage::Tick(float _DeltaTime)
{
	APage::Tick(_DeltaTime);

	switch (State)
	{
	case EPokemonUIState::TargetSelectionWait:
		TargetSelectionWaitTick(_DeltaTime);
		break;
	case EPokemonUIState::ActionSelectionWait:
		ActionSelectionWaitTick(_DeltaTime);
		break;
	case EPokemonUIState::SwitchSelectionWait:
		SwitchSelectionWaitTick(_DeltaTime);
		break;
	case EPokemonUIState::Switch:
		SwitchTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void UPokemonUILevelPage::TargetSelectionWaitTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(PrevMapLevelName, 0.3f, 0.3f);
		ActionCursor->SetCursor(0);
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
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (true == IsEntry(TargetCursor))
		{
			MemoryEntryCursor = TargetCursor;
		}

		MoveTargetCursor(UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1));
	}
}

void UPokemonUILevelPage::MoveTargetCursor(int _Cursor)
{
	TargetCursor = _Cursor;
	RefreshAllTargets();
}

void UPokemonUILevelPage::TargetSelect()
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
		TargetSelectionMsgBoxRenderer->SetActive(false);
		ActionSelectionMsgBoxRenderer->SetActive(true);
		ActionBoxRenderer->SetActive(true);
		ActionCursor->SetCursor(0);
	}
}

void UPokemonUILevelPage::ActionSelectionWaitTick(float _DeltaTime)
{
	if (UEngineInput::IsDown(VK_UP))
	{
		ActionCursor->DecCursor();
	}
	else if (UEngineInput::IsDown(VK_DOWN))
	{
		ActionCursor->IncCursor();
	}
	else if (UEngineInput::IsDown('X'))
	{
		State = EPokemonUIState::TargetSelectionWait;
		TargetSelectionMsgBoxRenderer->SetActive(true);
		ActionSelectionMsgBoxRenderer->SetActive(false);
		ActionBoxRenderer->SetActive(false);
	}
	else if (UEngineInput::IsDown('Z'))
	{
		ActionSelect();
	}
}

void UPokemonUILevelPage::ActionSelect()
{
	switch (ActionCursor->GetCursor())
	{
	case 0:
		// PokemonSummaryUI 레벨로 전환
	{
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(Global::PokemonSummaryUILevel);
	}
	break;
	case 1:
		// Switch 상태로 전환
		State = EPokemonUIState::SwitchSelectionWait;
		SwitchSelectionWaitStart();
		break;
	case 2:
		State = EPokemonUIState::TargetSelectionWait;
		TargetSelectionMsgBoxRenderer->SetActive(true);
		ActionSelectionMsgBoxRenderer->SetActive(false);
		ActionBoxRenderer->SetActive(false);
		break;
	default:
		break;
	}
}

void UPokemonUILevelPage::SwitchSelectionWaitStart()
{
	ActionBoxRenderer->SetActive(false);
	ActionSelectionMsgBoxRenderer->SetActive(false);
	SwitchSelectionMsgBoxRenderer->SetActive(true);
	SwitchFromCursor = TargetCursor;
	RefreshAllTargets();
}

void UPokemonUILevelPage::SwitchSelectionWaitTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBoxRenderer->SetActive(false);
		TargetSelectionMsgBoxRenderer->SetActive(true);
		RefreshAllTargets();
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		SwitchSelect();
		return;
	}

	int EntrySize = UPlayerData::GetPokemonEntrySize();
	if (true == UEngineInput::IsDown(VK_LEFT) && IsEntry(TargetCursor))
	{
		MemoryEntryCursor = TargetCursor;
		MoveTargetCursor(0);
	}

	if (true == UEngineInput::IsDown(VK_RIGHT) && IsFirst(TargetCursor))
	{
		MoveTargetCursor(MemoryEntryCursor);
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		if (true == IsEntry(TargetCursor))
		{
			MemoryEntryCursor = TargetCursor;
		}

		MoveTargetCursor(UPokemonMath::Mod(TargetCursor - 1, EntrySize + 1));
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		if (true == IsEntry(TargetCursor))
		{
			MemoryEntryCursor = TargetCursor;
		}

		MoveTargetCursor(UPokemonMath::Mod(TargetCursor + 1, EntrySize + 1));
	}
}

void UPokemonUILevelPage::SwitchSelect()
{
	if (TargetCursor == SwitchFromCursor || true == IsCancel(TargetCursor))
	{
		// 스위치 취소
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBoxRenderer->SetActive(false);
		TargetSelectionMsgBoxRenderer->SetActive(true);
		RefreshAllTargets();
		return;
	}

	State = EPokemonUIState::Switch;
	SwitchStart();
}

void UPokemonUILevelPage::SwitchStart()
{
	SwitchMoveState = ESwitchMoveState::Out;
	SwitchMoveTimer = SwitchMoveOutTime;

	if (true == IsFirst(SwitchFromCursor))
	{
		SwitchMoveSwitchFromRenderer = FirstRenderer;
		SwitchFromPrevPos = SwitchMoveSwitchFromRenderer->GetPosition();
		SwitchFromOutPos = SwitchFromPrevPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchMoveSwitchFromRenderer = EntryRenderers[SwitchFromCursor - 1];
		SwitchFromPrevPos = SwitchMoveSwitchFromRenderer->GetPosition();
		SwitchFromOutPos = SwitchFromPrevPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}

	if (true == IsFirst(TargetCursor))
	{
		SwitchMoveTargetRenderer = FirstRenderer;
		TargetPrevPos = SwitchMoveTargetRenderer->GetPosition();
		TargetOutPos = TargetPrevPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchMoveTargetRenderer = EntryRenderers[TargetCursor - 1];
		TargetPrevPos = SwitchMoveTargetRenderer->GetPosition();
		TargetOutPos = TargetPrevPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}

}

void UPokemonUILevelPage::SwitchTick(float _DeltaTime)
{
	// 교체가 전부 끝나면 상태 변경 후 메시지 박스 변경
	switch (SwitchMoveState)
	{
	case UPokemonUILevelPage::ESwitchMoveState::Out:
		SwitchMoveOutTick(_DeltaTime);
		break;
	case UPokemonUILevelPage::ESwitchMoveState::Wait:
		SwitchMoveWaitTick(_DeltaTime);
		break;
	case UPokemonUILevelPage::ESwitchMoveState::In:
		SwitchMoveInTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void UPokemonUILevelPage::SwitchMoveOutTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	float t = 1.0f - SwitchMoveTimer / SwitchMoveOutTime;
	SwitchMoveSwitchFromRenderer->SetPosition(UPokemonMath::Lerp(SwitchFromPrevPos, SwitchFromOutPos, t));
	SwitchMoveTargetRenderer->SetPosition(UPokemonMath::Lerp(TargetPrevPos, TargetOutPos, t));

	if (SwitchMoveTimer <= 0.0f)
	{
		SwitchMoveState = ESwitchMoveState::Wait;
		SwitchMoveTimer = SwitchMoveWaitTime;
		UPlayerData::SwapEntry(SwitchFromCursor, TargetCursor);
		RefreshAllTargets();
	}
}

void UPokemonUILevelPage::SwitchMoveWaitTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	if (SwitchMoveTimer <= 0.0f)
	{
		SwitchMoveState = ESwitchMoveState::In;
		SwitchMoveTimer = SwitchMoveInTime;
	}
}

void UPokemonUILevelPage::SwitchMoveInTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	float t = SwitchMoveTimer / SwitchMoveInTime;
	SwitchMoveSwitchFromRenderer->SetPosition(UPokemonMath::Lerp(SwitchFromPrevPos, SwitchFromOutPos, t));
	SwitchMoveTargetRenderer->SetPosition(UPokemonMath::Lerp(TargetPrevPos, TargetOutPos, t));

	if (SwitchMoveTimer <= 0.0f)
	{
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBoxRenderer->SetActive(false);
		TargetSelectionMsgBoxRenderer->SetActive(true);
		RefreshAllTargets();
	}
}

// Refresh 함수

void UPokemonUILevelPage::RefreshFirst()
{
	ETargetState TargetState = ETargetState::Empty;

	switch (State)
	{
	case EPokemonUIState::TargetSelectionWait:
	case EPokemonUIState::ActionSelectionWait:
		if (TargetCursor == 0)
		{
			TargetState = ETargetState::Focused;
		}
		else
		{
			TargetState = ETargetState::Unfocused;
		}
		break;
	case EPokemonUIState::SwitchSelectionWait:
	case EPokemonUIState::Switch:
		if (TargetCursor == 0)
		{
			TargetState = ETargetState::To;
		}
		else if (SwitchFromCursor == 0)
		{
			TargetState = ETargetState::From;
		}
		else
		{
			TargetState = ETargetState::Unfocused;
		}
		break;
	default:
		break;
	}

	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;

	switch (TargetState)
	{
	case UPokemonUILevelPage::ETargetState::Unfocused:
		ImageName = RN::PokemonUIFirstBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case UPokemonUILevelPage::ETargetState::Focused:
		ImageName = RN::PokemonUIFirstFocusedBox;
		PixelX = 2;
		PixelY = 18;
		break;
	case UPokemonUILevelPage::ETargetState::From:
		ImageName = RN::PokemonUIFirstFromBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case UPokemonUILevelPage::ETargetState::To:
		ImageName = RN::PokemonUIFirstToBox;
		PixelX = 2;
		PixelY = 18;
		break;
	default:
		MsgBoxAssert("Pokemon UI의 First의 상태가 Empty입니다.");
		break;
	}

	if (State != EPokemonUIState::Switch)
	{
		FirstRenderer->SetImage(ImageName);
		UPokemonUtil::PlaceImageOnScreen(
			FirstRenderer,
			EPivotType::LeftTop,
			PixelX, PixelY
		);
	}

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(0);

	FirstNameText->SetText(Pokemon.GetName());
	FirstLevelText->SetText(std::to_wstring(Pokemon.GetLevel()));
	FirstHpText->SetText(std::to_wstring(Pokemon.GetHp()));
	FirstCurHpText->SetText(std::to_wstring(Pokemon.GetCurHp()));
	FirstHpBar->SetMaxValue(Pokemon.GetHp());
	FirstHpBar->SetValue(Pokemon.GetCurHp());
	FirstPokemonIcon->SetPokemon(Pokemon);
	FirstGender->SetImage(Pokemon.GetGenderImageName());
}

void UPokemonUILevelPage::RefreshEntry(int _Index)
{
	ETargetState TargetState = ETargetState::Empty;

	switch (State)
	{
	case EPokemonUIState::TargetSelectionWait:
	case EPokemonUIState::ActionSelectionWait:
		if (false == IsEntry(_Index))
		{
			TargetState = ETargetState::Empty;
		}
		else if (TargetCursor == _Index)
		{
			TargetState = ETargetState::Focused;
		}
		else
		{
			TargetState = ETargetState::Unfocused;
		}
		break;
	case EPokemonUIState::SwitchSelectionWait:
	case EPokemonUIState::Switch:
		if (false == IsEntry(_Index))
		{
			TargetState = ETargetState::Empty;
		}
		else if (TargetCursor == _Index)
		{
			TargetState = ETargetState::To;
		}
		else if (SwitchFromCursor == _Index)
		{
			TargetState = ETargetState::From;
		}
		else
		{
			TargetState = ETargetState::Unfocused;
		}
		break;
	default:
		break;
	}

	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;


	switch (TargetState)
	{
	case UPokemonUILevelPage::ETargetState::Empty:
		ImageName = RN::PokemonUIEntryEmptyBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case UPokemonUILevelPage::ETargetState::Unfocused:
		ImageName = RN::PokemonUIEntryBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case UPokemonUILevelPage::ETargetState::Focused:
		ImageName = RN::PokemonUIEntryFocusedBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	case UPokemonUILevelPage::ETargetState::From:
		ImageName = RN::PokemonUIEntryFromBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case UPokemonUILevelPage::ETargetState::To:
		ImageName = RN::PokemonUIEntryToBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	default:
		break;
	}

	if (State != EPokemonUIState::Switch)
	{
		EntryRenderers[_Index - 1]->SetImage(ImageName);
		UPokemonUtil::PlaceImageOnScreen(
			EntryRenderers[_Index - 1],
			EPivotType::RightTop,
			PixelX, PixelY
		);
	}

	if (_Index < 0 || _Index >= UPlayerData::GetPokemonEntrySize())
	{
		return;
	}

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(_Index);
	EntryNameTexts[_Index - 1]->SetText(Pokemon.GetName());
	EntryLevelTexts[_Index - 1]->SetText(std::to_wstring(Pokemon.GetLevel()));
	EntryHpTexts[_Index - 1]->SetText(std::to_wstring(Pokemon.GetHp()));
	EntryCurHpTexts[_Index - 1]->SetText(std::to_wstring(Pokemon.GetCurHp()));
	EntryHpBars[_Index - 1]->SetMaxValue(Pokemon.GetHp());
	EntryHpBars[_Index - 1]->SetValue(Pokemon.GetCurHp());
	EntryPokemonIcons[_Index - 1]->SetPokemon(Pokemon);
	EntryGenders[_Index - 1]->SetImage(Pokemon.GetGenderImageName());
}

void UPokemonUILevelPage::RefreshCancel()
{
	ETargetState TargetState = ETargetState::Unfocused;

	if (true == IsCancel(TargetCursor))
	{
		TargetState = ETargetState::Focused;
	}

	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;

	switch (TargetState)
	{
	case UPokemonUILevelPage::ETargetState::Unfocused:
		ImageName = RN::PokemonUICancel;
		PixelX = -2;
		PixelY = -6;
		break;
	case UPokemonUILevelPage::ETargetState::Focused:
		ImageName = RN::PokemonUICancelFocused;
		PixelX = -2;
		PixelY = -4;
		break;
	default:
		MsgBoxAssert("Pokemon UI에서 Cancel의 상태가 From 또는 To입니다.");
		break;
	}

	CancelRenderer->SetImage(ImageName);
	UPokemonUtil::PlaceImageOnScreen(
		CancelRenderer,
		EPivotType::RightBot,
		PixelX, PixelY
	);
}

void UPokemonUILevelPage::RefreshAllTargets()
{
	RefreshFirst();
	for (int i = 1; i <= 5; ++i)
	{
		RefreshEntry(i);
	}
	RefreshCancel();
}
