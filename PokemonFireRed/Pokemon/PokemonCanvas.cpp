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

void APokemonCanvas::Init()
{
	// 데이터 초기화
	int EntrySize = UPlayerData::GetPokemonEntrySize();
	FirstBoxState = EBoxState::Focused;
	EntryBoxStates.clear();
	for (int i = 1; i < EntrySize; ++i)
	{
		EntryBoxStates.push_back(EBoxState::Unfocused);
	}
	for (int i = EntrySize; i < 6; ++i)
	{
		EntryBoxStates.push_back(EBoxState::Empty);
	}
	CancelBoxState = EBoxState::Unfocused;

	// 하위 요소 초기화
	ActionCursor->SetCursor(0);
	BattleActionCursor->SetCursor(0);
	RefreshAllTargets();

	// 렌더링 설정
	ActionSelectionMsgBox->SetActive(false);
	SwitchSelectionMsgBox->SetActive(false);
	ActionBox->SetActive(false);
	BattleActionBox->SetActive(false);
	BattleMsgBox->SetActive(false);
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

	BattleActionBox = CreateImageElement(Background, ERenderingOrder::UI4, EPivotType::RightBot, -1, -1);
	BattleActionBox->SetImage(RN::PokemonUIBattleActionBox);

	BattleMsgBox = CreateImageElement(Background,
		ERenderingOrder::UI6, EPivotType::RightBot, -1, -1);
	BattleMsgBox->SetImage(RN::PokemonUIBattleMsgBox);

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

	BattleActionCursor = CreateCursor(BattleActionBox, ERenderingOrder::UI5, EPivotType::LeftTop, 8, 11, RN::BlackCursor, 16);
	BattleActionCursor->SetOptionCount(3);

	// 배틀 메시지
	BattleMsg = CreateText(BattleMsgBox,
		ERenderingOrder::UI7, EPivotType::LeftTop, 7, 19,
		EAlignType::Left, EFontColor::Black3);
	BattleMsg->SetLineSpace(15);

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

int APokemonCanvas::GetActionCursor() const
{
	return ActionCursor->GetCursor();
}

void APokemonCanvas::SetActionCursor(int _Cursor)
{
	ActionCursor->SetCursor(_Cursor);
}

void APokemonCanvas::IncActionCursor()
{
	ActionCursor->IncCursor();
}

void APokemonCanvas::DecActionCursor()
{
	ActionCursor->DecCursor();
}

int APokemonCanvas::GetBattleActionCursor() const
{
	return BattleActionCursor->GetCursor();
}

void APokemonCanvas::SetBattleActionCursor(int _Cursor)
{
	BattleActionCursor->SetCursor(_Cursor);
}

void APokemonCanvas::IncBattleActionCursor()
{
	BattleActionCursor->IncCursor();
}

void APokemonCanvas::DecBattleActionCursor()
{
	BattleActionCursor->DecCursor();
}

AImageElement* APokemonCanvas::GetPokemonBox(int _Index)
{
	if (_Index == 0)
	{
		return FirstBox;
	}
	else if (_Index == UPlayerData::GetPokemonEntrySize())
	{
		MsgBoxAssert("CancelBox는 PokemonBox가 아닙니다.");
		return nullptr;
	}

	return EntryBoxes[_Index - 1];
}

bool APokemonCanvas::IsFirstBox(const AImageElement* _PokemonBox) const
{
	return FirstBox == _PokemonBox;
}

void APokemonCanvas::LerpPokemonBox(int _Index, const FVector& _Before, const FVector& _After, float _t)
{
	AImageElement* Mover = nullptr;

	if (_Index == 0)
	{
		Mover = FirstBox;
	}
	else if (_Index == UPlayerData::GetPokemonEntrySize())
	{
		MsgBoxAssert("CancelBox를 Lerp하려고 했습니다.");
	}
	else
	{
		Mover = EntryBoxes[_Index - 1];
	}
}

void APokemonCanvas::SetTargetSelectionMsgBoxActive(bool _Value)
{
	TargetSelectionMsgBox->SetActive(_Value);
}

void APokemonCanvas::SetActionSelectionMsgBoxActive(bool _Value)
{
	ActionSelectionMsgBox->SetActive(_Value);
}

void APokemonCanvas::SetSwitchSelectionMsgBoxActive(bool _Value)
{
	SwitchSelectionMsgBox->SetActive(_Value);
}

void APokemonCanvas::SetActionBoxActive(bool _Value)
{
	ActionBox->SetActive(_Value);
}

void APokemonCanvas::SetBattleActionBoxActive(bool _Value)
{
	BattleActionBox->SetActive(_Value);
}

void APokemonCanvas::SetBattleMsgBoxActive(bool _Value)
{
	BattleMsgBox->SetActive(_Value);
}

void APokemonCanvas::SetBattleMessage(std::wstring _Msg)
{
	BattleMsg->SetText(_Msg);
}

void APokemonCanvas::SetBoxState(int _BoxIndex, EBoxState _BoxState)
{
	if (_BoxIndex == 0)
	{
		FirstBoxState = _BoxState;
		return;
	}

	if (_BoxIndex == UPlayerData::GetPokemonEntrySize())
	{
		if (_BoxState == EBoxState::To)
		{
			// 취소 버튼은 Switch 대상이 될 수 없다.
			CancelBoxState = EBoxState::Focused;
			return;
		}

		CancelBoxState = _BoxState;
		return;
	}

	EntryBoxStates[_BoxIndex - 1] = _BoxState;
}

// Refresh 함수
void APokemonCanvas::RefreshFirst(bool _IsSwitchMode)
{
	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;

	switch (FirstBoxState)
	{
	case APokemonCanvas::EBoxState::Unfocused:
		ImageName = RN::PokemonUIFirstBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case APokemonCanvas::EBoxState::Focused:
		ImageName = RN::PokemonUIFirstFocusedBox;
		PixelX = 2;
		PixelY = 18;
		break;
	case APokemonCanvas::EBoxState::From:
		ImageName = RN::PokemonUIFirstFromBox;
		PixelX = 2;
		PixelY = 20;
		break;
	case APokemonCanvas::EBoxState::To:
		ImageName = RN::PokemonUIFirstToBox;
		PixelX = 2;
		PixelY = 18;
		break;
	default:
		MsgBoxAssert("Pokemon UI의 First의 상태가 Empty입니다.");
		break;
	}

	if (false == _IsSwitchMode)
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

void APokemonCanvas::RefreshEntry(int _Index, bool _IsSwitchMode)
{
	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;

	switch (EntryBoxStates[_Index - 1])
	{
	case APokemonCanvas::EBoxState::Empty:
		ImageName = RN::PokemonUIEntryEmptyBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::EBoxState::Unfocused:
		ImageName = RN::PokemonUIEntryBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::EBoxState::Focused:
		ImageName = RN::PokemonUIEntryFocusedBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::EBoxState::From:
		ImageName = RN::PokemonUIEntryFromBox;
		PixelX = -2;
		PixelY = 10 + 24 * (_Index - 1);
		break;
	case APokemonCanvas::EBoxState::To:
		ImageName = RN::PokemonUIEntryToBox;
		PixelX = -2;
		PixelY = 9 + 24 * (_Index - 1);
		break;
	default:
		break;
	}

	if (false == _IsSwitchMode) 
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
	std::string ImageName;
	int PixelX = 0;
	int PixelY = 0;

	switch (CancelBoxState)
	{
	case APokemonCanvas::EBoxState::Unfocused:
		ImageName = RN::PokemonUICancel;
		PixelX = -2;
		PixelY = -6;
		break;
	case APokemonCanvas::EBoxState::Focused:
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

void APokemonCanvas::RefreshAllTargets(bool _IsSwitchMode)
{
	RefreshFirst(_IsSwitchMode);
	for (int i = 1; i <= 5; ++i)
	{
		RefreshEntry(i, _IsSwitchMode);
	}
	RefreshCancel();
}
