#include "PokemonSummaryPage.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonString.h"
#include "PokemonLevel.h"
#include "PokemonUtil.h"
#include "PlayerData.h"

APokemonSummaryPage::APokemonSummaryPage()
{
}

APokemonSummaryPage::~APokemonSummaryPage()
{
}

void APokemonSummaryPage::BeginPlay()
{
	APage::BeginPlay();

	// 최상위 요소
	Background = CreateImageRenderer(ERenderingOrder::Background);
	Background->SetImage(RN::PokemonSummaryUIBackground);
	UPokemonUtil::PlaceImageOnScreen(Background);

	Nav = CreateImageRenderer(ERenderingOrder::LowerUI);
	Nav->SetImage(RN::PokemonSummaryUINavInfo);
	UPokemonUtil::PlaceImageOnScreen(Nav);

	CommonBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	CommonBox->SetImage(RN::PokemonSummaryUICommonBox);
	UPokemonUtil::PlaceImageOnScreen(CommonBox, EPivotType::LeftTop, 0, 16);

	InfoBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	InfoBox->SetImage(RN::PokemonSummaryUIInfoBox);
	UPokemonUtil::PlaceImageOnScreen(InfoBox, EPivotType::LeftTop, 0, 16);

	SkillsBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	SkillsBox->SetImage(RN::PokemonSummaryUISkillsBox);
	UPokemonUtil::PlaceImageOnScreen(SkillsBox, EPivotType::LeftTop, 0, 16);

	MovesBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	MovesBox->SetImage(RN::PokemonSummaryUIMovesBox);
	UPokemonUtil::PlaceImageOnScreen(MovesBox, EPivotType::LeftTop, 119, 16);

	MovesDetailBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	MovesDetailBox->SetImage(RN::PokemonSummaryUIMovesDetailBox);
	UPokemonUtil::PlaceImageOnScreen(MovesDetailBox, EPivotType::LeftTop, 0, 49);

	SmallCommonBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	SmallCommonBox->SetImage(RN::PokemonSummaryUISmallCommonBox);
	UPokemonUtil::PlaceImageOnScreen(SmallCommonBox, EPivotType::LeftTop, 0, 16);

	// CommonBox 요소
	NameText = CreateText(CommonBox, L"NAME", EPivotType::LeftTop, EAlignType::Left, 40, 13);
	LevelText = CreateText(CommonBox, L"99", EPivotType::LeftTop, EAlignType::Left, 14, 13);
	GenderIcon = CreateImageElement(CommonBox, EPivotType::LeftTop, 105, 3);
	FrontImage = CreatePokemonElement(CommonBox, EPokemonElementType::Front, EPivotType::RightTop, -28, 17);

	// InfoBox 요소
	PokedexNoText = CreateText(InfoBox, L"000", EPivotType::RightTop, EAlignType::Left, -72, 15, EFontColor::Black2);
	SpeciesNameText = CreateText(InfoBox, L"UNDEFINED", EPivotType::RightTop, EAlignType::Left, -72, 30, EFontColor::Black2);
	TrainerText = CreateText(InfoBox, L"RED", EPivotType::RightTop, EAlignType::Left, -72, 60, EFontColor::Black2);
	IdNoText = CreateText(InfoBox, L"00000", EPivotType::RightTop, EAlignType::Left, -72, 75, EFontColor::Black2);
	ItemText = CreateText(InfoBox, L"NONE", EPivotType::RightTop, EAlignType::Left, -72, 90, EFontColor::Black2);
	TrainerMemo = CreateText(InfoBox, L"XXXX Nature.", EPivotType::LeftBot, EAlignType::Left, 8, -33, EFontColor::Black2);

	for (int i = 0; i < 2; ++i)
	{
		AImageElement* TypeImage = CreateImageElement(InfoBox, EPivotType::RightTop, -41 + 34 * i, 35);
		TypeImage->SetImage(RN::TypePlaceHolder);
		TypeImages.push_back(TypeImage);
	}

	// SkillsBox 요소
	HpText = CreateText(SkillsBox, L"27/30", EPivotType::RightTop, EAlignType::Right, -3, 15, EFontColor::Black2);
	AtkText = CreateText(SkillsBox, L"16", EPivotType::RightTop, EAlignType::Right, -3, 33, EFontColor::Black2);
	DefText = CreateText(SkillsBox, L"14", EPivotType::RightTop, EAlignType::Right, -3, 46, EFontColor::Black2);
	SpAtkText = CreateText(SkillsBox, L"13", EPivotType::RightTop, EAlignType::Right, -3, 59, EFontColor::Black2);
	SpDefText = CreateText(SkillsBox, L"12", EPivotType::RightTop, EAlignType::Right, -3, 72, EFontColor::Black2);
	SpeedText = CreateText(SkillsBox, L"17", EPivotType::RightTop, EAlignType::Right, -3, 85, EFontColor::Black2);
	AccExpText = CreateText(SkillsBox, L"570", EPivotType::RightTop, EAlignType::Right, -3, 98, EFontColor::Black2);
	NextLevelExpText = CreateText(SkillsBox, L"172", EPivotType::RightTop, EAlignType::Right, -3, 111, EFontColor::Black2);
	AbilityText = CreateText(SkillsBox, L"KEEN EYE", EPivotType::LeftTop, EAlignType::Left, 74, 124, EFontColor::Black2);
	AbilityExplainText = CreateText(SkillsBox, L"Prevents loss of accuracy.", EPivotType::LeftBot, EAlignType::Left, 10, -5, EFontColor::Black2);
	HpBar = CreateScrollBar(SkillsBox, EScrollType::Hp, 100, 100, EPivotType::RightTop, -55, 18);
	ExpBar = CreateScrollBar(SkillsBox, EScrollType::Exp, 100, 100, EPivotType::RightBot, -71, -29);

	// MovesBox 요소
	for (int i = 0; i < 4; ++i)
	{
		AText* MoveNameText = CreateText(MovesBox, L"-", EPivotType::LeftTop, EAlignType::Left, 43, 16 + 28 * i, EFontColor::Black2);
		MoveNameTexts.push_back(MoveNameText);

		AText* MovePPText = CreateText(MovesBox, L"99/99", EPivotType::RightTop, EAlignType::Left, -33, 27 + 28 * i, EFontColor::Black2);
		MovePPTexts.push_back(MovePPText);

		AImageElement* MoveTypeImage = CreateImageElement(MovesBox, EPivotType::LeftTop, 4, 5 + 28 * i);
		MoveTypeImage->SetImage(RN::TypeBug);
		MoveTypeImages.push_back(MoveTypeImage);
	}

	// SmallCommonBox 요소
	SCBMiniStopImage = CreatePokemonElement(SmallCommonBox, EPokemonElementType::MiniStop, EPivotType::LeftTop, 6, 8);
	SCBGenderIcon = CreateImageElement(SmallCommonBox, EPivotType::LeftTop, 105, 3);
	SCBGenderIcon->SetImage(RN::BigGenderMarkMale);
	SCBNameText = CreateText(SmallCommonBox, L"POKEMON", EPivotType::LeftTop, EAlignType::Left, 40, 13, EFontColor::White);

	for (int i = 0; i < 2; ++i)
	{
		AImageElement* TypeImage = CreateImageElement(SmallCommonBox, EPivotType::LeftTop, 48 + 36 * i, 19);
		TypeImage->SetImage(RN::TypeNormal);
		SCBTypeImages.push_back(TypeImage);
	}

	// MovesDetailBox 요소
	MovePowerText = CreateText(MovesDetailBox, L"120", EPivotType::RightTop, EAlignType::Right, -46, 19, EFontColor::Black);
	MoveAccuracyText = CreateText(MovesDetailBox, L"100", EPivotType::RightTop, EAlignType::Right, -46, 33, EFontColor::Black);
	MoveExplainText = CreateText(MovesDetailBox, L"Hello\nWorld", EPivotType::LeftBot, EAlignType::Left, 7, -50, EFontColor::Black);
	MoveFocusCursor = CreateCursor(MovesDetailBox, RN::PokemonSummaryUIMoveFocus, 0, 5, EPivotType::RightTop, 119, -31, 28);
}

void APokemonSummaryPage::RefreshAll()
{
	EPokedexNo PokedexNo = Pokemon->GetPokedexNo();

	// CommonBox
	NameText->SetText(Pokemon->GetNameW());
	LevelText->SetText(std::to_wstring(Pokemon->GetLevel()));
	IdNoText->SetText(UPlayerData::GetIdNoW());
	FrontImage->SetPokemon(Pokemon);
	GenderIcon->SetImage(Pokemon->GetBigGenderImageName());

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


void APokemonSummaryPage::RefreshMoveDetailBox()
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

void APokemonSummaryPage::Reset()
{
	State = EPokemonSummaryPageState::Info;
	Background->SetActive(true);
	CommonBox->SetActive(true);
	SmallCommonBox->SetActive(false);
	InfoBox->SetActive(true);
	SkillsBox->SetActive(false);
	MovesBox->SetActive(false);
	MovesDetailBox->SetActive(false);
	Nav->SetActive(true);
	Nav->SetImage(RN::PokemonSummaryUINavInfo);
}

void APokemonSummaryPage::Tick(float _DeltaTime)
{
	APage::Tick(_DeltaTime);

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

void APokemonSummaryPage::InfoTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		UEventManager::ChangeLevelFade(GetWorld(), Global::PokemonUILevel);
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

void APokemonSummaryPage::SkillsTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::ChangeLevelFade(GetWorld(), Global::PokemonUILevel);
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

void APokemonSummaryPage::MovesTick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		UEventManager::ChangeLevelFade(GetWorld(), Global::PokemonUILevel);
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

void APokemonSummaryPage::MovesDetailTick(float _DeltaTime)
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
