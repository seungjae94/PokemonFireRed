#include "PokemonLevel.h"
#include <EnginePlatform/EngineInput.h>
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
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::Player, { 77, 137 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::Player, true);
	}

	// 상록 시티로 이동
	if (true == UEngineInput::IsDown('2'))
	{
		USoundManager::PlayBgm(RN::BgmViridianCity);
		UEventManager::FadeChangeLevel(Global::ExteriorPalletTownLevel, true);
		UEventManager::SetPoint(Global::ExteriorPalletTownLevel, EN::Player, { 82, 80 });
		UEventManager::SetDirection(Global::ExteriorPalletTownLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPalletTownLevel, EN::Player, true);
	}

	// 상록숲으로 이동
	if (true == UEngineInput::IsDown('3'))
	{
		USoundManager::PlayBgm(RN::BgmViridianForest);
		UEventManager::FadeChangeLevel(Global::ExteriorViridianForestLevel, true);
		UEventManager::SetPoint(Global::ExteriorViridianForestLevel, EN::Player, { 35, 60 });
		UEventManager::SetDirection(Global::ExteriorViridianForestLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorViridianForestLevel, EN::Player, true);
	}

	// 회색 시티로 이동
	if (true == UEngineInput::IsDown('4'))
	{
		USoundManager::PlayBgm(RN::BgmPewterCity);
		UEventManager::FadeChangeLevel(Global::ExteriorPewterCityLevel, true);
		UEventManager::SetPoint(Global::ExteriorPewterCityLevel, EN::Player, { 19, 21 });
		UEventManager::SetDirection(Global::ExteriorPewterCityLevel, EN::Player, FTileVector::Down);
		UEventManager::SetActive(Global::ExteriorPewterCityLevel, EN::Player, true);
	}
}

void UPokemonLevel::ApplyDamageCheat()
{
	DamageCheatStage = UPokemonMath::Mod(++DamageCheatStage, 4);

	if (0 == DamageCheatStage)
	{
		UEngineDebug::OutPutDebugText("Damage x1");
		Global::DamageBonusCoeff = 1.0f;
	}
	else if (1 == DamageCheatStage)
	{
		UEngineDebug::OutPutDebugText("Damage x2");
		Global::DamageBonusCoeff = 2.0f;
	}
	else if (2 == DamageCheatStage)
	{
		UEngineDebug::OutPutDebugText("Damage x4");
		Global::DamageBonusCoeff = 4.0f;
	}
	else if (3 == DamageCheatStage)
	{
		UEngineDebug::OutPutDebugText("Damage x16");
		Global::DamageBonusCoeff = 16.0f;
	}
}

void UPokemonLevel::ApplyExpCheat()
{
	ExpCheatStage = UPokemonMath::Mod(++ExpCheatStage, 4);

	if (0 == ExpCheatStage)
	{
		UEngineDebug::OutPutDebugText("Exp x1");
		Global::ExpBonusCoeff = 1.0f;
	}
	else if (1 == ExpCheatStage)
	{
		UEngineDebug::OutPutDebugText("Exp x2");
		Global::ExpBonusCoeff = 2.0f;
	}
	else if (2 == ExpCheatStage)
	{
		UEngineDebug::OutPutDebugText("Exp x4");
		Global::ExpBonusCoeff = 4.0f;
	}
	else if (3 == ExpCheatStage)
	{
		UEngineDebug::OutPutDebugText("Exp x16");
		Global::ExpBonusCoeff = 16.0f;
	}
}

void UPokemonLevel::ApplyEncounterCheat()
{
	EncounterCheatStage = UPokemonMath::Mod(++EncounterCheatStage, 3);

	if (0 == EncounterCheatStage)
	{
		UEngineDebug::OutPutDebugText("야생 포켓몬 출현율 x1");
		Global::WildBattleFrequency = 1.0f;
	}
	else if (1 == EncounterCheatStage)
	{
		UEngineDebug::OutPutDebugText("야생 포켓몬 출현율 x8");
		Global::WildBattleFrequency = 8.0f;
	}
	else if (2 == EncounterCheatStage)
	{
		UEngineDebug::OutPutDebugText("야생 포켓몬 출현율 x0");
		Global::WildBattleFrequency = 0.0f;
	}
}

void UPokemonLevel::ApplyCatchCheat()
{
	CatchCheatStage = UPokemonMath::Mod(++CatchCheatStage, 4);

	if (0 == CatchCheatStage)
	{
		UEngineDebug::OutPutDebugText("포획률 x1");
		Global::CatchRateBonusCoeff = 1.0f;
	}
	else if (1 == CatchCheatStage)
	{
		UEngineDebug::OutPutDebugText("포획률 x16");
		Global::CatchRateBonusCoeff = 16.0f;
	}
	else if (2 == CatchCheatStage)
	{
		UEngineDebug::OutPutDebugText("포획률 x0.25");
		Global::CatchRateBonusCoeff = 0.25f;
	}
	else if (3 == CatchCheatStage)
	{
		UEngineDebug::OutPutDebugText("포획률 x0.5");
		Global::CatchRateBonusCoeff = 0.25f;
	}
}
