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
}

void APokemonSummaryPage::Tick(float _DeltaTime)
{
	APage::Tick(_DeltaTime);

	int a = 0;
}
