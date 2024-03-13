#include "PokemonLevel.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "FadeCanvas.h"
#include "BattleLevel.h"

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
	UEventManager::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_F1))
	{
		if (true == Global::IsCheatOn)
		{
			Global::IsCheatOn = false;
			Global::DamageBonusCoeff = 1.0f;
			Global::ExpBonusCoeff = 1.0f;
			Global::WildBattleFrequency = 1.0f;
			Global::CatchRateBonusCoeff = 1;
			UEngineDebug::OutPutDebugText("Cheat off!");
		}
		else
		{
			Global::IsCheatOn = true;
			Global::DamageBonusCoeff = Global::CheatDamageBonusCoeff;
			Global::ExpBonusCoeff = Global::CheatExpBonusCoeff;
			Global::WildBattleFrequency = Global::CheatWildBattleFrequency;
			Global::CatchRateBonusCoeff = Global::CheatCatchRateBonusCoeff;
			UEngineDebug::OutPutDebugText("Cheat on!");
		}
	}

	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(this);

	if (nullptr != BattleLevel)
	{
		return;
	}

	// 태초 마을로 이동
	if (true == UEngineInput::IsDown('1'))
	{
		UEventManager::FadeChangeLevel(Global::ExteriorPalletTownLevel, true);
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::Player, { 77, 137 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::Player, true);
	}

	// 상록 시티로 이동
	if (true == UEngineInput::IsDown('2'))
	{
		UEventManager::FadeChangeLevel(Global::ExteriorPalletTownLevel, true);
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::Player, { 82, 80 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::Player, true);
	}

	// 상록숲으로 이동
	if (true == UEngineInput::IsDown('3'))
	{
		UEventManager::FadeChangeLevel(Global::ExteriorViridianForestLevel, true);
		UEventManager::SetPoint(Global::ExteriorViridianForestLevel, EN::Player, { 35, 60 });
		UEventManager::SetDirection(Global::ExteriorViridianForestLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorViridianForestLevel, EN::Player, true);
	}

	// 회색 시티로 이동
	if (true == UEngineInput::IsDown('4'))
	{
		UEventManager::FadeChangeLevel(Global::ExteriorPewterCityLevel, true);
		UEventManager::SetPoint(Global::ExteriorPewterCityLevel, EN::Player, { 19, 21 });
		UEventManager::SetDirection(Global::ExteriorPewterCityLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPewterCityLevel, EN::Player, true);
	}
}

