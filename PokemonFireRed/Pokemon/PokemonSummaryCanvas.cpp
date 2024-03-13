#include "PokemonSummaryCanvas.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonString.h"
#include "PokemonLevel.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

APokemonSummaryCanvas::APokemonSummaryCanvas()
{
}

APokemonSummaryCanvas::~APokemonSummaryCanvas()
{
}

void APokemonSummaryCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 최상위 요소
	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::PokemonSummaryUIBackground);

	Nav = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 0);
	Nav->SetImage(RN::PokemonSummaryUINavInfo);

	CommonBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 16);
	CommonBox->SetImage(RN::PokemonSummaryUICommonBox);

	InfoBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 16);
	InfoBox->SetImage(RN::PokemonSummaryUIInfoBox);

	SkillsBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 16);
	SkillsBox->SetImage(RN::PokemonSummaryUISkillsBox);

	MovesBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 119, 16);
	MovesBox->SetImage(RN::PokemonSummaryUIMovesBox);

	MovesDetailBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 49);
	MovesDetailBox->SetImage(RN::PokemonSummaryUIMovesDetailBox);

	SmallCommonBox = CreateImageElement(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 0, 16);
	SmallCommonBox->SetImage(RN::PokemonSummaryUISmallCommonBox);

	// CommonBox 요소
	NameText = CreateText(CommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 40, 13, EAlignType::Left);
	LevelText = CreateText(CommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 14, 13, EAlignType::Left);
	GenderIcon = CreateImageElement(CommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 105, 3);
	FrontImage = CreateImageElement(CommonBox, ERenderingOrder::UI2, EPivotType::RightTop, -28, 5, EImageElementType::PokemonFront);
	StatusImage = CreateImageElement(CommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 6, 18);

	// InfoBox 요소
	PokedexNoText = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -72, 15, EAlignType::Left, EFontColor::Black2);
	SpeciesNameText = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -72, 30, EAlignType::Left, EFontColor::Black2);
	TrainerText = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -72, 60, EAlignType::Left, EFontColor::Black2);
	IdNoText = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -72, 75, EAlignType::Left, EFontColor::Black2);
	ItemText = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -72, 90, EAlignType::Left, EFontColor::Black2);
	TrainerMemo = CreateText(InfoBox, ERenderingOrder::UI2, EPivotType::LeftBot, 8, -33, EAlignType::Left, EFontColor::Black2);

	for (int i = 0; i < 2; ++i)
	{
		AImageElement* TypeImage = CreateImageElement(InfoBox, ERenderingOrder::UI2, EPivotType::RightTop, -41 + 34 * i, 35);
		TypeImage->SetImage(RN::TypePlaceHolder);
		TypeImages.push_back(TypeImage);
	}

	// SkillsBox 요소
	HpText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 15, EAlignType::Right, EFontColor::Black2);
	AtkText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 33, EAlignType::Right, EFontColor::Black2);
	DefText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 46, EAlignType::Right, EFontColor::Black2);
	SpAtkText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 59, EAlignType::Right, EFontColor::Black2);
	SpDefText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 72, EAlignType::Right, EFontColor::Black2);
	SpeedText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 85, EAlignType::Right, EFontColor::Black2);
	AccExpText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 98, EAlignType::Right, EFontColor::Black2);
	NextLevelExpText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -3, 111, EAlignType::Right, EFontColor::Black2);
	AbilityText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::LeftBot, 73, -19, EAlignType::Left, EFontColor::Black2);
	AbilityExplainText = CreateText(SkillsBox, ERenderingOrder::UI2, EPivotType::LeftBot, 10, -5, EAlignType::Left, EFontColor::Black2);
	HpBar = CreateScrollBar(SkillsBox, ERenderingOrder::UI2, EPivotType::RightTop, -55, 18, EScrollType::Hp);
	ExpBar = CreateScrollBar(SkillsBox, ERenderingOrder::UI2, EPivotType::RightBot, -71, -29, EScrollType::Exp);

	// MovesBox 요소
	for (int i = 0; i < 4; ++i)
	{
		AText* MoveNameText = CreateText(MovesBox, ERenderingOrder::UI2, EPivotType::LeftTop, 43, 16 + 28 * i, EAlignType::Left, EFontColor::Black2);
		MoveNameTexts.push_back(MoveNameText);

		AText* MovePPText = CreateText(MovesBox, ERenderingOrder::UI2, EPivotType::RightTop, -33, 27 + 28 * i, EAlignType::Left, EFontColor::Black2);
		MovePPTexts.push_back(MovePPText);

		AImageElement* MoveTypeImage = CreateImageElement(MovesBox, ERenderingOrder::UI2, EPivotType::LeftTop, 4, 5 + 28 * i);
		MoveTypeImage->SetImage(RN::TypeBug);
		MoveTypeImages.push_back(MoveTypeImage);
	}

	// SmallCommonBox 요소
	SCBMiniStopImage = CreateImageElement(SmallCommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 6, 8, EImageElementType::PokemonMiniStop);
	SCBGenderIcon = CreateImageElement(SmallCommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 105, 3);
	SCBGenderIcon->SetImage(RN::BigGenderMarkMale);
	SCBNameText = CreateText(SmallCommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 40, 13, EAlignType::Left, EFontColor::White);

	for (int i = 0; i < 2; ++i)
	{
		AImageElement* TypeImage = CreateImageElement(SmallCommonBox, ERenderingOrder::UI2, EPivotType::LeftTop, 48 + 36 * i, 19);
		TypeImage->SetImage(RN::TypeNormal);
		SCBTypeImages.push_back(TypeImage);
	}

	// MovesDetailBox 요소
	MovePowerText = CreateText(MovesDetailBox, ERenderingOrder::UI2, EPivotType::RightTop, -46, 19, EAlignType::Right, EFontColor::Black);
	MoveAccuracyText = CreateText(MovesDetailBox, ERenderingOrder::UI2, EPivotType::RightTop, -46, 33, EAlignType::Right, EFontColor::Black);
	MoveExplainText = CreateText(MovesDetailBox, ERenderingOrder::UI2, EPivotType::LeftBot, 7, -50, EAlignType::Left, EFontColor::Black);
	MoveFocusCursor = CreateCursor(MovesDetailBox, ERenderingOrder::UI3, EPivotType::RightTop, 119, -31, RN::PokemonSummaryUIMoveFocus, 28);
	MoveFocusCursor->SetOptionCount(5);
}

void APokemonSummaryCanvas::RefreshAll()
{
	EPokemonId PokedexNo = Pokemon->GetPokedexNo();

	// CommonBox
	NameText->SetText(Pokemon->GetNameW());
	LevelText->SetText(std::to_wstring(Pokemon->GetLevel()));
	IdNoText->SetText(UPlayerData::GetIdNoW());
	FrontImage->SetPokemon(Pokemon);
	GenderIcon->SetImage(Pokemon->GetBigGenderImageName());
	StatusImage->SetImage(Pokemon->GetStatusImageName());

	// InfoBox
	PokedexNoText->SetText(Pokemon->GetPokedexNoW());
	SpeciesNameText->SetText(UPokemonString::ToUpperW(Pokemon->GetSpeciesNameW()));
	TrainerText->SetText(L"RED");
	TrainerMemo->SetText(Pokemon->GetNatureNameW() + L" nature.");

	for (int i = 0; i < Pokemon->GetTypeCount(); ++i)
	{
		TypeImages[i]->SetImage(Pokemon->GetTypeImageName(i));
	}
	for (int i = Pokemon->GetTypeCount(); i < 2; ++i)
	{
		TypeImages[i]->SetImage(RN::TypePlaceHolder);
	}

	// SkillsBox
	HpText->SetText(Pokemon->GetCurHpW() + L"/" + Pokemon->GetHpW());
	AtkText->SetText(Pokemon->GetAtkW());
	DefText->SetText(Pokemon->GetDefW());
	SpAtkText->SetText(Pokemon->GetSpAtkW());
	SpDefText->SetText(Pokemon->GetSpDefW());
	SpeedText->SetText(Pokemon->GetSpeedW());
	AccExpText->SetText(Pokemon->GetAccExpW());
	NextLevelExpText->SetText(Pokemon->GetNextLevelExpW());
	AbilityText->SetText(Pokemon->GetAbilityUpperW());
	AbilityExplainText->SetText(Pokemon->GetAbilityExplainW());
	HpBar->SetMaxValue(Pokemon->GetHp());
	HpBar->SetValue(Pokemon->GetCurHp());
	ExpBar->SetMaxValue(Pokemon->GetExpSize());
	ExpBar->SetValue(Pokemon->GetExp());

	// MovesBox
	for (int i = 0; i < Pokemon->GetMoveCount(); ++i)
	{
		MoveNameTexts[i]->SetText(Pokemon->GetMoveNameW(i));
		MovePPTexts[i]->SetText(Pokemon->GetMoveCurPPW(i) + L"/" + Pokemon->GetMovePPW(i));
		MoveTypeImages[i]->SetImage(Pokemon->GetMoveTypeImageName(i));
	}
	for (int i = Pokemon->GetMoveCount(); i < 4; ++i)
	{
		MoveNameTexts[i]->SetText(L"-");
		MovePPTexts[i]->SetText(L"--");
		MoveTypeImages[i]->SetImage(RN::TypePlaceHolder);
	}

	// SmalCommonBox
	SCBMiniStopImage->SetPokemon(Pokemon);
	SCBGenderIcon->SetImage(Pokemon->GetBigGenderImageName());
	SCBNameText->SetText(Pokemon->GetSpeciesNameW());

	for (int i = 0; i < Pokemon->GetTypeCount(); ++i)
	{
		SCBTypeImages[i]->SetImage(Pokemon->GetTypeImageName(i));
	}
	for (int i = Pokemon->GetTypeCount(); i < 2; ++i)
	{
		SCBTypeImages[i]->SetImage(RN::TypePlaceHolder);
	}

	// MovesDetailBox
	RefreshMoveDetailBox();
}


void APokemonSummaryCanvas::RefreshMoveDetailBox()
{
	int Cursor = MoveFocusCursor->GetCursor();
	if (Cursor >= Pokemon->GetMoveCount())
	{
		MovePowerText->SetText(L"");
		MoveAccuracyText->SetText(L"");
		MoveExplainText->SetText(L"");
	}
	else
	{
		int MovePower = Pokemon->GetMovePower(Cursor);
		if (0 == MovePower)
		{
			MovePowerText->SetText(L"---");
		}
		else
		{
			MovePowerText->SetText(Pokemon->GetMovePowerW(Cursor));
		}

		MoveAccuracyText->SetText(Pokemon->GetMoveAccuracyW(Cursor));
		MoveExplainText->SetText(Pokemon->GetMoveExplainW(Cursor));
	}
}

void APokemonSummaryCanvas::Reset()
{
	// 데이터 초기화
	MoveFocusCursor->SetCursor(0);

	// UI를 다시 그린다.
	State = EPokemonSummaryPageState::Info;
	Nav->SetImage(RN::PokemonSummaryUINavInfo);
	RefreshAll();

	// 렌더링 여부 설정
	Background->SetActive(true);
	Nav->SetActive(true);
	CommonBox->SetActive(true);
	InfoBox->SetActive(true);
	SkillsBox->SetActive(false);
	MovesBox->SetActive(false);
	SmallCommonBox->SetActive(false);
	MovesDetailBox->SetActive(false);
}

void APokemonSummaryCanvas::SetPrevLevelName(std::string_view _PrevLevelName)
{
	PrevLevelName = _PrevLevelName;
}

void APokemonSummaryCanvas::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	switch (State)
	{
	case EPokemonSummaryPageState::Info:
		InfoTick(_DeltaTime);
		break;
	case EPokemonSummaryPageState::Skills:
		SkillsTick(_DeltaTime);
		break;
	case EPokemonSummaryPageState::Moves:
		MovesTick(_DeltaTime);
		break;
	case EPokemonSummaryPageState::MovesDetail:
		MovesDetailTick(_DeltaTime);
		break;
	default:
		break;
	}
}

void APokemonSummaryCanvas::InfoTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevLevelName, false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		State = EPokemonSummaryPageState::Skills;
		Nav->SetImage(RN::PokemonSummaryUINavSkills);
		InfoBox->SetActive(false);
		SkillsBox->SetActive(true);
		return;
	}
}

void APokemonSummaryCanvas::SkillsTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevLevelName, false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		State = EPokemonSummaryPageState::Info;
		Nav->SetImage(RN::PokemonSummaryUINavInfo);
		InfoBox->SetActive(true);
		SkillsBox->SetActive(false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		State = EPokemonSummaryPageState::Moves;
		Nav->SetImage(RN::PokemonSummaryUINavMoves);
		MovesBox->SetActive(true);
		SkillsBox->SetActive(false);
		return;
	}
}

void APokemonSummaryCanvas::MovesTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevLevelName, false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		State = EPokemonSummaryPageState::Skills;
		Nav->SetImage(RN::PokemonSummaryUINavSkills);
		SkillsBox->SetActive(true);
		MovesBox->SetActive(false);
		return;
	}

	if (true == UEngineInput::IsDown('Z'))
	{
		State = EPokemonSummaryPageState::MovesDetail;
		Nav->SetImage(RN::PokemonSummaryUINavMovesDetail);
		CommonBox->SetActive(false);
		SmallCommonBox->SetActive(true);
		MovesDetailBox->SetActive(true);
	}
}

void APokemonSummaryCanvas::MovesDetailTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		State = EPokemonSummaryPageState::Moves;
		Nav->SetImage(RN::PokemonSummaryUINavMoves);
		CommonBox->SetActive(true);
		SmallCommonBox->SetActive(false);
		MovesDetailBox->SetActive(false);
		return;
	}

	if (true == MoveFocusCursor->IsLast() && true == UEngineInput::IsDown('Z'))
	{
		State = EPokemonSummaryPageState::Moves;
		Nav->SetImage(RN::PokemonSummaryUINavMoves);
		CommonBox->SetActive(true);
		SmallCommonBox->SetActive(false);
		MovesDetailBox->SetActive(false);
		return;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		MoveFocusCursor->IncCursor();
		while (true)
		{
			int Cursor = MoveFocusCursor->GetCursor();

			if (Cursor < Pokemon->GetMoveCount() || true == MoveFocusCursor->IsLast())
			{
				break;
			}

			MoveFocusCursor->IncCursor();
		}

		RefreshMoveDetailBox();
		return;
	}

	if (true == UEngineInput::IsUp(VK_UP))
	{
		MoveFocusCursor->DecCursor();
		while (true)
		{
			int Cursor = MoveFocusCursor->GetCursor();

			if (Cursor < Pokemon->GetMoveCount() || true == MoveFocusCursor->IsLast())
			{
				break;
			}

			MoveFocusCursor->DecCursor();
		}

		RefreshMoveDetailBox();
		return;
	}
}
