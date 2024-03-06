#include "PokemonCanvas.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "Global.h"
#include "PokemonUtil.h"
#include "PokemonUILevel.h"
#include "Pokemon.h"

APokemonCanvas::APokemonCanvas()
{
}

APokemonCanvas::~APokemonCanvas()
{
}

void APokemonCanvas::BeginPlay()
{
	// 백그라운드
	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::PokemonUIBackground);

	// 최상위 요소
	TargetSelectionMsgBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 2, -2);
	TargetSelectionMsgBox->SetImage(RN::PokemonUITargetSelectionMsgBox);

	ActionSelectionMsgBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 2, -2);
	ActionSelectionMsgBox->SetImage(RN::PokemonUIActionSelectionMsgBox);

	SwitchSelectionMsgBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftBot, 2, -2);
	SwitchSelectionMsgBox->SetImage(RN::PokemonUISwitchSelectionMsgBox);

	ActionBox = CreateImageElement(Background, ERenderingOrder::UI4, EPivotType::RightBot, -1, -1);
	ActionBox->SetImage(RN::PokemonUIActionBox);

	FirstBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 0);
	EntryBoxes.reserve(6);
	for (int i = 1; i < 6; ++i)
	{
		AImageElement* EntryBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::RightTop, 0, 0);
		EntryBoxes.push_back(EntryBox);
	}
	CancelButton = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::RightBot, 0, 0);

	// 액션 선택창
	ActionCursor = CreateCursor(ActionBox, ERenderingOrder::UI5, EPivotType::LeftTop, 8, 11, RN::BlackCursor, 16);
	ActionCursor->SetOptionCount(3);

	// 엔트리
	UPokemon& First = UPlayerData::GetPokemonInEntry(0);
	FirstNameText = CreateText(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -53, -28, EAlignType::Left, EFontColor::WhiteGray, EFontSize::Mini);
	FirstLevelText = CreateText(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -37, -19, EAlignType::Left, EFontColor::WhiteGray, EFontSize::Mini);
	FirstHpText = CreateText(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -5, -3, EAlignType::Right, EFontColor::WhiteGray, EFontSize::Mini);
	FirstCurHpText = CreateText(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -25, -3, EAlignType::Right, EFontColor::WhiteGray, EFontSize::Mini);
	FirstHpBar = CreateScrollBar(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -53, -15, EScrollType::Hp);
	FirstPokemonIcon = CreateImageElement(FirstBox, ERenderingOrder::UI2, EPivotType::LeftBot, -2, -23, EImageElementType::PokemonMiniMove);
	FirstGender = CreateImageElement(FirstBox, ERenderingOrder::UI2, EPivotType::RightBot, -9, -19);
	FirstStatus = CreateImageElement(FirstBox, ERenderingOrder::UI3, EPivotType::RightBot, -28, -19);

	for (int i = 1; i < UPlayerData::GetPokemonEntrySize(); ++i)
	{
		UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(i);

		AText* NameText = CreateText(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::RightBot, -119, -9, EAlignType::Left, EFontColor::WhiteGray, EFontSize::Mini);
		AText* LevelText = CreateText(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::LeftBot, 48, 0, EAlignType::Left, EFontColor::WhiteGray, EFontSize::Mini);
		AText* HpText = CreateText(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::RightBot, -5, 0, EAlignType::Right, EFontColor::WhiteGray, EFontSize::Mini);
		AText* CurHpText = CreateText(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::RightBot, -25, 0, EAlignType::Right, EFontColor::WhiteGray, EFontSize::Mini);
		AScrollBar* HpBar = CreateScrollBar(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::RightBot, -53, -13, EScrollType::Hp);
		AImageElement* Icon = CreateImageElement(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::LeftBot, -3, 3, EImageElementType::PokemonMiniMove);
		AImageElement* Gender = CreateImageElement(EntryBoxes[i - 1], ERenderingOrder::UI2, EPivotType::RightBot, -72, 0);
		AImageElement* Status = CreateImageElement(EntryBoxes[i - 1], ERenderingOrder::UI3, EPivotType::RightBot, -91, 0);

		EntryNameTexts.push_back(NameText);
		EntryLevelTexts.push_back(LevelText);
		EntryHpTexts.push_back(HpText);
		EntryCurHpTexts.push_back(CurHpText);
		EntryHpBars.push_back(HpBar);
		EntryPokemonIcons.push_back(Icon);
		EntryGenders.push_back(Gender);
		EntryStatuses.push_back(Status);
	}
}

void APokemonCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

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

void APokemonCanvas::TargetSelectionWaitTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevMapLevelName, false);
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

void APokemonCanvas::MoveTargetCursor(int _Cursor)
{
	TargetCursor = _Cursor;
	RefreshAllTargets();
}

void APokemonCanvas::TargetSelect()
{
	if (TargetCursor == UPlayerData::GetPokemonEntrySize())
	{
		// 캔슬 버튼 입력
		UEventManager::FadeChangeLevel(PrevMapLevelName, false);
	}
	else
	{
		State = EPokemonUIState::ActionSelectionWait;
		TargetSelectionMsgBox->SetActive(false);
		ActionSelectionMsgBox->SetActive(true);
		ActionBox->SetActive(true);
		ActionCursor->SetCursor(0);
	}
}

void APokemonCanvas::ActionSelectionWaitTick(float _DeltaTime)
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
		TargetSelectionMsgBox->SetActive(true);
		ActionSelectionMsgBox->SetActive(false);
		ActionBox->SetActive(false);
	}
	else if (UEngineInput::IsDown('Z'))
	{
		ActionSelect();
	}
}

void APokemonCanvas::ActionSelect()
{
	switch (ActionCursor->GetCursor())
	{
	case 0:
		// PokemonSummaryUI 레벨로 전환
		UEventManager::FadeChangeLevel(Global::PokemonSummaryUILevel, false);
		break;
	case 1:
		// Switch 상태로 전환
		State = EPokemonUIState::SwitchSelectionWait;
		SwitchSelectionWaitStart();
		break;
	case 2:
		State = EPokemonUIState::TargetSelectionWait;
		TargetSelectionMsgBox->SetActive(true);
		ActionSelectionMsgBox->SetActive(false);
		ActionBox->SetActive(false);
		break;
	default:
		break;
	}
}

void APokemonCanvas::SwitchSelectionWaitStart()
{
	ActionBox->SetActive(false);
	ActionSelectionMsgBox->SetActive(false);
	SwitchSelectionMsgBox->SetActive(true);
	SwitchFromCursor = TargetCursor;
	RefreshAllTargets();
}

void APokemonCanvas::SwitchSelectionWaitTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBox->SetActive(false);
		TargetSelectionMsgBox->SetActive(true);
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

void APokemonCanvas::SwitchSelect()
{
	if (TargetCursor == SwitchFromCursor || true == IsCancel(TargetCursor))
	{
		// 스위치 취소
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBox->SetActive(false);
		TargetSelectionMsgBox->SetActive(true);
		RefreshAllTargets();
		return;
	}

	State = EPokemonUIState::Switch;
	SwitchStart();
}

void APokemonCanvas::SwitchStart()
{
	SwitchMoveState = ESwitchMoveState::Out;
	SwitchMoveTimer = SwitchMoveOutTime;

	if (true == IsFirst(SwitchFromCursor))
	{
		SwitchMoveSwitchFrom = FirstBox;
		SwitchFromPrevPos = SwitchMoveSwitchFrom->GetRelativePosition();
		SwitchFromOutPos = SwitchFromPrevPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchMoveSwitchFrom = EntryBoxes[SwitchFromCursor - 1];
		SwitchFromPrevPos = SwitchMoveSwitchFrom->GetRelativePosition();
		SwitchFromOutPos = SwitchFromPrevPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}

	if (true == IsFirst(TargetCursor))
	{
		SwitchMoveTarget = FirstBox;
		TargetPrevPos = SwitchMoveTarget->GetRelativePosition();
		TargetOutPos = TargetPrevPos - FVector(0.5f * Global::FloatScreenX, 0.0f);
	}
	else
	{
		SwitchMoveTarget = EntryBoxes[TargetCursor - 1];
		TargetPrevPos = SwitchMoveTarget->GetRelativePosition();
		TargetOutPos = TargetPrevPos + FVector(0.75f * Global::FloatScreenX, 0.0f);
	}

}

void APokemonCanvas::SwitchTick(float _DeltaTime)
{
	// 교체가 전부 끝나면 상태 변경 후 메시지 박스 변경
	switch (SwitchMoveState)
	{
	case APokemonCanvas::ESwitchMoveState::Out:
		SwitchMoveOutTick(_DeltaTime);
		break;
	case APokemonCanvas::ESwitchMoveState::Wait:
		SwitchMoveWaitTick(_DeltaTime);
		break;
	case APokemonCanvas::ESwitchMoveState::In:
		SwitchMoveInTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void APokemonCanvas::SwitchMoveOutTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	float t = 1.0f - SwitchMoveTimer / SwitchMoveOutTime;
	SwitchMoveSwitchFrom->SetRelativePosition(UPokemonMath::Lerp(SwitchFromPrevPos, SwitchFromOutPos, t));
	SwitchMoveTarget->SetRelativePosition(UPokemonMath::Lerp(TargetPrevPos, TargetOutPos, t));

	if (SwitchMoveTimer <= 0.0f)
	{
		SwitchMoveState = ESwitchMoveState::Wait;
		SwitchMoveTimer = SwitchMoveWaitTime;
		UPlayerData::SwapEntry(SwitchFromCursor, TargetCursor);
		RefreshAllTargets();
	}
}

void APokemonCanvas::SwitchMoveWaitTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	if (SwitchMoveTimer <= 0.0f)
	{
		SwitchMoveState = ESwitchMoveState::In;
		SwitchMoveTimer = SwitchMoveInTime;
	}
}

void APokemonCanvas::SwitchMoveInTick(float _DeltaTime)
{
	SwitchMoveTimer -= _DeltaTime;

	float t = SwitchMoveTimer / SwitchMoveInTime;
	SwitchMoveSwitchFrom->SetRelativePosition(UPokemonMath::Lerp(SwitchFromPrevPos, SwitchFromOutPos, t));
	SwitchMoveTarget->SetRelativePosition(UPokemonMath::Lerp(TargetPrevPos, TargetOutPos, t));

	if (SwitchMoveTimer <= 0.0f)
	{
		State = EPokemonUIState::TargetSelectionWait;
		SwitchSelectionMsgBox->SetActive(false);
		TargetSelectionMsgBox->SetActive(true);
		RefreshAllTargets();
	}
}

// Refresh 함수

void APokemonCanvas::RefreshFirst()
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
	case APokemonCanvas::ETargetState::Unfocused:
		ImageName = RN::PokemonUIFirstBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case APokemonCanvas::ETargetState::Focused:
		ImageName = RN::PokemonUIFirstFocusedBox;
		PixelX = 2;
		PixelY = 18;
		break;
	case APokemonCanvas::ETargetState::From:
		ImageName = RN::PokemonUIFirstFromBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case APokemonCanvas::ETargetState::To:
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
		FirstBox->SetImage(ImageName);
		FirstBox->SetRelativePosition(PixelX, PixelY);
	}

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(0);

	FirstNameText->SetText(Pokemon.GetNameW());
	
	std::wstring LevelText = L"";
	if (Pokemon.GetStatusId() == EPokemonStatus::Normal)
	{
		LevelText = Pokemon.GetLevelW();
	}
	FirstLevelText->SetText(LevelText);
	
	FirstHpText->SetText(std::to_wstring(Pokemon.GetHp()));
	FirstCurHpText->SetText(std::to_wstring(Pokemon.GetCurHp()));
	FirstHpBar->SetMaxValue(Pokemon.GetHp());
	FirstHpBar->SetValue(Pokemon.GetCurHp());
	FirstPokemonIcon->SetPokemon(Pokemon);
	FirstGender->SetImage(Pokemon.GetGenderImageName());
	FirstStatus->SetImage(Pokemon.GetStatusImageName());
}

void APokemonCanvas::RefreshEntry(int _Index)
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
	case APokemonCanvas::ETargetState::Empty:
		ImageName = RN::PokemonUIEntryEmptyBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::ETargetState::Unfocused:
		ImageName = RN::PokemonUIEntryBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::ETargetState::Focused:
		ImageName = RN::PokemonUIEntryFocusedBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::ETargetState::From:
		ImageName = RN::PokemonUIEntryFromBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::ETargetState::To:
		ImageName = RN::PokemonUIEntryToBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	default:
		break;
	}

	if (State != EPokemonUIState::Switch)
	{
		EntryBoxes[_Index - 1]->SetImage(ImageName);
		EntryBoxes[_Index - 1]->SetRelativePosition(PixelX, PixelY);
	}

	if (_Index < 0 || _Index >= UPlayerData::GetPokemonEntrySize())
	{
		return;
	}

	const UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(_Index);
	EntryNameTexts[_Index - 1]->SetText(Pokemon.GetNameW());

	std::wstring LevelText = L"";
	if (Pokemon.GetStatusId() == EPokemonStatus::Normal)
	{
		LevelText = Pokemon.GetLevelW();
	}
	EntryLevelTexts[_Index - 1]->SetText(LevelText);

	EntryHpTexts[_Index - 1]->SetText(Pokemon.GetHpW());
	EntryCurHpTexts[_Index - 1]->SetText(Pokemon.GetCurHpW());
	EntryHpBars[_Index - 1]->SetMaxValue(Pokemon.GetHp());
	EntryHpBars[_Index - 1]->SetValue(Pokemon.GetCurHp());
	EntryPokemonIcons[_Index - 1]->SetPokemon(Pokemon);
	EntryGenders[_Index - 1]->SetImage(Pokemon.GetGenderImageName());
	EntryStatuses[_Index - 1]->SetImage(Pokemon.GetStatusImageName());
}

void APokemonCanvas::RefreshCancel()
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
	case APokemonCanvas::ETargetState::Unfocused:
		ImageName = RN::PokemonUICancel;
		PixelX = -2;
		PixelY = -6;
		break;
	case APokemonCanvas::ETargetState::Focused:
		ImageName = RN::PokemonUICancelFocused;
		PixelX = -2;
		PixelY = -4;
		break;
	default:
		MsgBoxAssert("Pokemon UI에서 Cancel의 상태가 From 또는 To입니다.");
		break;
	}

	CancelButton->SetImage(ImageName);
	CancelButton->SetRelativePosition(PixelX, PixelY);
}

void APokemonCanvas::RefreshAllTargets()
{
	RefreshFirst();
	for (int i = 1; i <= 5; ++i)
	{
		RefreshEntry(i);
	}
	RefreshCancel();
}
