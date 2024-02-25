#include "TrainerCardPage.h"
#include <EnginePlatform/EngineInput.h>
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

	Background = CreateImageRenderer(ERenderingOrder::LowerUI);
	Background->SetImage(RN::MaleTrainerCard);
	UPokemonUtil::PlaceImageOnScreen(
		Background,
		EPivotType::LeftTop,
		0, 0
	);
}

void ATrainerCardPage::Tick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		UEventManager::ChangeLevelFade(GetWorld(), PrevMapLevelName);
		return;
	}
}
