#include "PokemonSummaryPage.h"

APokemonSummaryPage::APokemonSummaryPage()
{
}

APokemonSummaryPage::~APokemonSummaryPage()
{
}

void APokemonSummaryPage::BeginPlay()
{
	APage::BeginPlay();

	Background = CreateImageRenderer(ERenderingOrder::Background);
	Background->SetImage(RN::PokemonSummaryUIBackground);
	PlaceImage(Background);

	NavInfo = CreateImageRenderer(ERenderingOrder::LowerUI);
	NavInfo->SetImage(RN::PokemonSummaryUINavInfo);
	PlaceImage(NavInfo);

	CommonBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	CommonBox->SetImage(RN::PokemonSummaryUICommonBox);
	PlaceImage(CommonBox, EPivotType::LeftTop, 0, 16);

	InfoBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	InfoBox->SetImage(RN::PokemonSummaryUIInfoBox);
	PlaceImage(InfoBox, EPivotType::LeftTop, 0, 16);

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

	// InfoBox 요소
	NoText = CreateText(
		InfoBox,
		L"000",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 16,
		EFontColor::Black
	);
	APokemonText* SpeciesNameText = CreateText(
		InfoBox,
		L"UNDEFINED",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 30,
		EFontColor::Black
	);

	// std::list<UImageRenderer*> TypeIcons;

	APokemonText* TrainerText = CreateText(
		InfoBox,
		L"RED",
		EPivotType::RightTop,
		EAlignType::Left,
		-72, 60,
		EFontColor::Black
	);

	APokemonText* TrainerMemo = CreateText(
		InfoBox,
		L"XXXX Nature.",
		EPivotType::LeftBot,
		EAlignType::Left,
		8, -33,
		EFontColor::Black
	);
}

void APokemonSummaryPage::Tick(float _DeltaTime)
{
	APage::Tick(_DeltaTime);
}
