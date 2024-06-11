#include "TrainerCardCanvas.h"
#include <EnginePlatform/EngineInput.h>
#include "SoundManager.h"
#include "UserData.h"
#include "TrainerCardUILevel.h"
#include "PokemonUtil.h"

ATrainerCardCanvas::ATrainerCardCanvas()
{
}

ATrainerCardCanvas::~ATrainerCardCanvas()
{
}

void ATrainerCardCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	Background = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftTop, 0, 0);
	Background->SetImage(RN::MaleTrainerCard);

	IdNo = CreateText(Background, ERenderingOrder::UI1, EPivotType::RightTop, -60, 29, EAlignType::Left, EFontColor::Gray);
	Nickname = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 63, 48, EAlignType::Left, EFontColor::Gray);
	Money = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 143, 75, EAlignType::Right, EFontColor::Gray);
	PokedexOwned = CreateText(Background, ERenderingOrder::UI1, EPivotType::LeftTop, 143, 91, EAlignType::Right, EFontColor::Gray);
}

void ATrainerCardCanvas::RefreshAll()
{
	IdNo->SetText(std::to_wstring(UUserData::GetIdNo()));
	Nickname->SetText(UUserData::GetNickNameW());
	Money->SetText(std::to_wstring(UUserData::GetMoney()) + L"G");
	PokedexOwned->SetText(std::to_wstring(UUserData::GetOwnedPokemonCount()));
}

void ATrainerCardCanvas::Tick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		USoundManager::PlaySE(RN::SEClick);
		UEventManager::FadeChangeLevel(PrevMapLevelName);
		return;
	}
}
