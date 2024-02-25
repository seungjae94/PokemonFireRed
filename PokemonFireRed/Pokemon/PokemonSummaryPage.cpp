#include "PokemonSummaryPage.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonString.h"
#include "PokemonLevel.h"
#include "PokemonUtil.h"

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

	// CommonBox 요소
	NameText =  CreateText(
		CommonBox,
		L"NAME",
		EPivotType::LeftTop,
		EAlignType::Left,
		40, 13
	);
	LevelText = CreateText(
		CommonBox,
		L"99",
		EPivotType::LeftTop,
		EAlignType::Left,
		14, 13
	);
	FrontImage = CreatePokemonElement(
		CommonBox,
		EPokemonElementType::Front,
		EPivotType::RightTop,
		-28, 17
	);

	// InfoBox 요소
	PokedexNoText = CreateText(
		InfoBox,
		L"000",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 15,
		EFontColor::Black
	);
	SpeciesNameText = CreateText(
		InfoBox,
		L"UNDEFINED",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 30,
		EFontColor::Black
	);

	TrainerText = CreateText(
		InfoBox,
		L"RED",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 60,
		EFontColor::Black
	);

	IdNoText = CreateText(
		InfoBox,
		L"00000",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 75,
		EFontColor::Black
	);

	ItemText = CreateText(
		InfoBox,
		L"NONE",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 90,
		EFontColor::Black
	);

	TrainerMemo = CreateText(
		InfoBox,
		L"XXXX Nature.",
		EPivotType::LeftBot,
		EAlignType::Left,
		8, -33,
		EFontColor::Black
	);

	for (int i = 0; i < 2; ++i)
	{
		AImageElement* TypeImage = CreateImageElement(
			InfoBox,
			EPivotType::RightTop,
			-41 + 34 * i, 35
		);
		TypeImage->SetImage(RN::TypePlaceHolder);
		TypeImages.push_back(TypeImage);
	}

	// SkillsBox 요소
	HpText = CreateText(SkillsBox, L"27/30", EPivotType::RightTop, EAlignType::Right, -3, 15, EFontColor::Black);
	AtkText = CreateText(SkillsBox, L"16", EPivotType::RightTop, EAlignType::Right, -3, 33, EFontColor::Black);
	DefText = CreateText(SkillsBox, L"14", EPivotType::RightTop, EAlignType::Right, -3, 46, EFontColor::Black);
	SpAtkText = CreateText(SkillsBox, L"13", EPivotType::RightTop, EAlignType::Right, -3, 59, EFontColor::Black);
	SpDefText = CreateText(SkillsBox, L"12", EPivotType::RightTop, EAlignType::Right, -3, 72, EFontColor::Black);
	SpeedText = CreateText(SkillsBox, L"17", EPivotType::RightTop, EAlignType::Right, -3, 85, EFontColor::Black);
	AccExpText = CreateText(SkillsBox, L"570", EPivotType::RightTop, EAlignType::Right, -3, 98, EFontColor::Black);
	NextLevelExpText = CreateText(SkillsBox, L"172", EPivotType::RightTop, EAlignType::Right, -3, 111, EFontColor::Black);
	AbilityText = CreateText(SkillsBox, L"KEEN EYE", EPivotType::LeftTop, EAlignType::Left, 74, 124, EFontColor::Black);
	AbilityExplainText = CreateText(SkillsBox, L"Prevents loss of accuracy.", EPivotType::LeftBot, EAlignType::Left, 10, -5, EFontColor::Black);
	HpBar = CreateScrollBar(SkillsBox, EScrollType::Hp, 100, 100, EPivotType::RightTop, -55, 18);
	ExpBar = CreateScrollBar(SkillsBox, EScrollType::Exp, 100, 100, EPivotType::RightBot, -71, -29);
}

void APokemonSummaryPage::RefreshAll()
{
	EPokedexNo PokedexNo = Pokemon->GetPokedexNo();
	std::wstring PokedexNoString = std::to_wstring(static_cast<int>(PokedexNo));
	ENature Nature = Pokemon->GetNature();
	std::vector<EPokemonType> Types = Pokemon->GetTypes();
	EGender Gender = Pokemon->GetGender();

	// CommonBox
	NameText->SetText(Pokemon->GetNameW());
	LevelText->SetText(std::to_wstring(Pokemon->GetLevel()));
	FrontImage->SetPokemon(Pokemon);
	if (nullptr == GenderIcon)
	{
		GenderIcon = CreateImageRenderer(ERenderingOrder::UpperUI);
	}
	GenderIcon->SetImage(Pokemon->GetBigGenderImageName());
	UPokemonUtil::PlaceImageOnScreen(GenderIcon, EPivotType::LeftTop, 105, 19);

	// InfoBox
	PokedexNoText->SetText(UPokemonString::PadLeft(PokedexNoString, 3, L'0'));
	SpeciesNameText->SetText(Pokemon->GetSpeciesW());
	TrainerText->SetText(L"RED");
	TrainerMemo->SetText(Pokemon->GetNatureW() + L" nature.");

	std::vector<std::string> TypeImageNames = Pokemon->GetTypeImageNames();
	for (int i = 0; i < 2; ++i)
	{
		TypeImages[i]->SetImage(TypeImageNames[i]);
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
}

void APokemonSummaryPage::Reset()
{
	State = EPokemonSummaryPageState::Info;
	Background->SetActive(true);
	CommonBox->SetActive(true);
	InfoBox->SetActive(true);
	SkillsBox->SetActive(false);
	//MovesBox->SetActive(false);
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
		break;
	case EPokemonSummaryPageState::MovesDetail:
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
}
