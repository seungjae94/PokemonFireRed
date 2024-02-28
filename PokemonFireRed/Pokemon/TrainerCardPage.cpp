#include "TrainerCardPage.h"
#include <EnginePlatform/EngineInput.h>
#include "PlayerData.h"
#include "TrainerCardUILevel.h"
#include "PokemonUtil.h"

ATrainerCardPage::ATrainerCardPage()
{
}

ATrainerCardPage::~ATrainerCardPage()
{
}

void ATrainerCardPage::BeginPlay()
{
	APage::BeginPlay();

	Background = CreateImageRenderer(ERenderingOrder::UI0);
	Background->SetImage(RN::MaleTrainerCard);
	UPokemonUtil::PlaceImageOnScreen(
		Background,
		EPivotType::LeftTop,
		0, 0
	);

	IdNo = CreateText(
		Background,
		L"99999",
		EPivotType::RightTop,
		EAlignType::Left,
		-60, 29,
		EFontColor::Gray
	);

	Nickname = CreateText(
		Background,
		L"RED",
		EPivotType::LeftTop,
		EAlignType::Left,
		63, 48,
		EFontColor::Gray
	);

	Money = CreateText(
		Background,
		L"99999G",
		EPivotType::LeftTop,
		EAlignType::Right,
		143, 75,
		EFontColor::Gray
	);

	PokedexOwned = CreateText(
		Background,
		L"0",
		EPivotType::LeftTop,
		EAlignType::Right,
		143, 91,
		EFontColor::Gray
	);
}

void ATrainerCardPage::RefreshAll()
{
	IdNo->SetText(std::to_wstring(UPlayerData::GetIdNo()));
	Nickname->SetText(UPlayerData::GetNickNameW());
	Money->SetText(std::to_wstring(UPlayerData::GetMoney()) + L"G");
	PokedexOwned->SetText(std::to_wstring(UPlayerData::GetOwnedPokemonCount()));
}

void ATrainerCardPage::Tick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		UEventManager::FadeChangeLevel(PrevMapLevelName);
		return;
	}
}
