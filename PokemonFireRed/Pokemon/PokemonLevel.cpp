#include "PokemonLevel.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineDebug.h>
#include <EngineCore/EngineCore.h>
#include "EventManager.h"
#include "FadeCanvas.h"
#include "BattleLevel.h"

int UPokemonLevel::DamageCheatStage = 0;
int UPokemonLevel::ExpCheatStage = 0;
int UPokemonLevel::CatchCheatStage = 0;
int UPokemonLevel::EncounterCheatStage = 0;

UPokemonLevel::UPokemonLevel()
{
}

UPokemonLevel::~UPokemonLevel()
{
}

void UPokemonLevel::BeginPlay()
{
	// 타이틀 레벨, 튜토리얼 레벨을 포함하는 공용 UI 엘리먼트 생성
	FadeCanvas = SpawnCommonCanvas<AFadeCanvas>(Global::FadeCanvas);
	FadeCanvas->SetActive(false);

	ADialogueWindow* DialogueWindow = SpawnActor<ADialogueWindow>();
	UEventManager::AddDialogueWindow(DialogueWindow);
	DialogueWindow->SetActive(false);

	if (nullptr == DebugCanvas)
	{
		DebugCanvas = SpawnActor<ADebugCanvas>();
	}

	// 게임 공용 트리거 생성
	SpawnEventTrigger<AFadeLevelChanger>(Global::FadeLevelChanger);
}

void UPokemonLevel::LevelStart(ULevel* _PrevLevel)
{
	ULevel::LevelStart(_PrevLevel);
}

void UPokemonLevel::LevelEnd(ULevel* _NextLevel)
{
	ULevel::LevelEnd(_NextLevel);
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	
	float TimeScaleValue = 1.0f;
	if (true == UEngineInput::IsPress(VK_SPACE))
	{
		TimeScaleValue = 2.0f;
	}

	UEventManager::Tick(_DeltaTime * TimeScaleValue);
	SetAllTimeScale(TimeScaleValue);

	if (true == UEngineInput::IsDown(VK_F2))
	{
		DebugCanvas->Refresh();
		DebugCanvas->SetActive(!DebugCanvas->IsActive());
	}

	if (true == UEngineInput::IsDown('Q'))
	{
		ApplyDamageCheat();
	}

	if (true == UEngineInput::IsDown('W'))
	{
		ApplyExpCheat();
	}

	if (true == UEngineInput::IsDown('E'))
	{
		ApplyEncounterCheat();
	}

	if (true == UEngineInput::IsDown('R'))
	{
		ApplyCatchCheat();
	}

	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(this);

	if (nullptr != BattleLevel)
	{
		return;
	}

	// 태초 마을로 이동
	if (true == UEngineInput::IsDown('1'))
	{
		USoundManager::PlayBgm(RN::BgmPalletTown);
		UEventManager::FadeChangeLevel(Global::ExteriorPalletTownLevel, true);
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, { 77, 137 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, true);
	}

	// 상록 시티로 이동
	if (true == UEngineInput::IsDown('2'))
	{
		USoundManager::PlayBgm(RN::BgmViridianCity);
		UEventManager::FadeChangeLevel(Global::ExteriorPalletTownLevel, true);
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, { 82, 80 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::PlayerCharacter, true);
	}

	// 상록숲으로 이동
	if (true == UEngineInput::IsDown('3'))
	{
		USoundManager::PlayBgm(RN::BgmViridianForest);
		UEventManager::FadeChangeLevel(Global::ExteriorViridianForestLevel, true);
		UEventManager::SetPoint(Global::ExteriorViridianForestLevel, EN::PlayerCharacter, { 35, 60 });
		UEventManager::SetDirection(Global::ExteriorViridianForestLevel, EN::PlayerCharacter, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorViridianForestLevel, EN::PlayerCharacter, true);
	}

	// 회색 시티로 이동
	if (true == UEngineInput::IsDown('4'))
	{
		USoundManager::PlayBgm(RN::BgmPewterCity);
		UEventManager::FadeChangeLevel(Global::ExteriorPewterCityLevel, true);
		UEventManager::SetPoint(Global::ExteriorPewterCityLevel, EN::PlayerCharacter, { 19, 21 });
		UEventManager::SetDirection(Global::ExteriorPewterCityLevel, EN::PlayerCharacter, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPewterCityLevel, EN::PlayerCharacter, true);
	}
}

void UPokemonLevel::ApplyDamageCheat()
{
	DamageCheatStage = UPokemonMath::Mod(++DamageCheatStage, 4);

	if (0 == DamageCheatStage)
	{
		Global::DamageBonusCoeff = 1.0f;
	}
	else if (1 == DamageCheatStage)
	{
		Global::DamageBonusCoeff = 2.0f;
	}
	else if (2 == DamageCheatStage)
	{
		Global::DamageBonusCoeff = 4.0f;
	}
	else if (3 == DamageCheatStage)
	{
		Global::DamageBonusCoeff = 16.0f;
	}

	DebugCanvas->Refresh();
}

void UPokemonLevel::ApplyExpCheat()
{
	ExpCheatStage = UPokemonMath::Mod(++ExpCheatStage, 4);

	if (0 == ExpCheatStage)
	{
		Global::ExpBonusCoeff = 1.0f;
	}
	else if (1 == ExpCheatStage)
	{
		Global::ExpBonusCoeff = 2.0f;
	}
	else if (2 == ExpCheatStage)
	{
		Global::ExpBonusCoeff = 4.0f;
	}
	else if (3 == ExpCheatStage)
	{
		Global::ExpBonusCoeff = 16.0f;
	}

	DebugCanvas->Refresh();
}

void UPokemonLevel::ApplyEncounterCheat()
{
	EncounterCheatStage = UPokemonMath::Mod(++EncounterCheatStage, 3);

	if (0 == EncounterCheatStage)
	{
		Global::EncounterRateBonusCoeff = 1.0f;
	}
	else if (1 == EncounterCheatStage)
	{
		Global::EncounterRateBonusCoeff = 8.0f;
	}
	else if (2 == EncounterCheatStage)
	{
		Global::EncounterRateBonusCoeff = 0.0f;
	}

	DebugCanvas->Refresh();
}

void UPokemonLevel::ApplyCatchCheat()
{
	CatchCheatStage = UPokemonMath::Mod(++CatchCheatStage, 4);

	if (0 == CatchCheatStage)
	{
		Global::CatchRateBonusCoeff = 1.0f;
	}
	else if (1 == CatchCheatStage)
	{
		Global::CatchRateBonusCoeff = 16.0f;
	}
	else if (2 == CatchCheatStage)
	{
		Global::CatchRateBonusCoeff = 0.25f;
	}
	else if (3 == CatchCheatStage)
	{
		Global::CatchRateBonusCoeff = 0.5f;
	}

	DebugCanvas->Refresh();
}
