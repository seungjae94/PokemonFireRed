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

	Background = CreateImageRenderer(ERenderingOrder::Background);
	Background->SetImage(RN::PokemonSummaryUIBackground);
	UPokemonUtil::PlaceImageOnScreen(Background);

	NavInfo = CreateImageRenderer(ERenderingOrder::LowerUI);
	NavInfo->SetImage(RN::PokemonSummaryUINavInfo);
	UPokemonUtil::PlaceImageOnScreen(NavInfo);

	CommonBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	CommonBox->SetImage(RN::PokemonSummaryUICommonBox);
	UPokemonUtil::PlaceImageOnScreen(CommonBox, EPivotType::LeftTop, 0, 16);

	InfoBox = CreateImageRenderer(ERenderingOrder::LowerUI);
	InfoBox->SetImage(RN::PokemonSummaryUIInfoBox);
	UPokemonUtil::PlaceImageOnScreen(InfoBox, EPivotType::LeftTop, 0, 16);

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
}

void APokemonSummaryPage::RefreshAll()
{
	EPokedexNo PokedexNo = Pokemon->GetPokedexNo();
	std::wstring PokedexNoString = std::to_wstring(static_cast<int>(PokedexNo));
	ENature Nature = Pokemon->GetNature();
	std::list<EPokemonType> Types = Pokemon->GetTypes();
	EGender Gender = Pokemon->GetGender();

	NameText->SetText(Pokemon->GetName());
	LevelText->SetText(std::to_wstring(Pokemon->GetLevel()));
	PokedexNoText->SetText(UPokemonString::PadLeft(PokedexNoString, 3, L'0'));
	SpeciesNameText->SetText(UPokemon::GetSpeciesNameW(PokedexNo));
	TrainerText->SetText(L"RED");
	TrainerMemo->SetText(UPokemon::GetNatureNameW(Nature) + L" nature.");
	FrontImage->SetPokemon(Pokemon);

	// 타입 이미지 렌더링
	if (TypeIcons.size() > 0)
	{
		for (UImageRenderer* Icon : TypeIcons)
		{
			Icon->Destroy();
		}
		TypeIcons.clear();
	}

	int TypeIndex = 0;
	for (EPokemonType Type : Types)
	{
		UImageRenderer* Renderer = CreateImageRenderer(ERenderingOrder::UpperUI);
		std::string IconName = UPokemon::GetTypeImageName(Type);
		Renderer->SetImage(IconName);
		UPokemonUtil::PlaceImageOnScreen(Renderer, EPivotType::RightTop, -41 + TypeIndex * 34, 51);
		TypeIcons.push_back(Renderer);
		++TypeIndex;
	}

	// 성별 이미지 렌더링
	if (nullptr == GenderIcon)
	{
		GenderIcon = CreateImageRenderer(ERenderingOrder::UpperUI);
	}
	GenderIcon->SetImage(UPokemon::GetBigGenderImageName(Gender));
	UPokemonUtil::PlaceImageOnScreen(GenderIcon, EPivotType::LeftTop, 105, 19);
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
		UPokemonLevel* CurLevel = dynamic_cast<UPokemonLevel*>(GetWorld());
		CurLevel->ChangeLevelFade(Global::PokemonUILevel, 0.3f, 0.3f);
	}
}
