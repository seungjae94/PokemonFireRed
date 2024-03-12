#include "PokemonLevel.h"
#include <EnginePlatform/EngineInput.h>
#include "EventManager.h"
#include "FadeCanvas.h"

UPokemonLevel::UPokemonLevel()
{
}

UPokemonLevel::~UPokemonLevel()
{
}

void UPokemonLevel::BeginPlay()
{
	// Ÿ��Ʋ ����, Ʃ�丮�� ������ �����ϴ� ���� UI ������Ʈ ����
	FadeCanvas = SpawnCommonCanvas<AFadeCanvas>(Global::FadeCanvas);
	FadeCanvas->SetActive(false);

	ADialogueWindow* DialogueWindow = SpawnActor<ADialogueWindow>();
	UEventManager::AddDialogueWindow(DialogueWindow);
	DialogueWindow->SetActive(false);

	// ���� ���� Ʈ���� ����
	SpawnEventTrigger<AFadeLevelChanger>(Global::FadeLevelChanger);
}

void UPokemonLevel::LevelStart(ULevel* _PrevLevel)
{
	
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
			UEngineDebug::OutPutDebugText("Cheat off!");
		}
		else
		{
			Global::IsCheatOn = true;
			Global::DamageBonusCoeff = Global::CheatDamageBonusCoeff;
			Global::ExpBonusCoeff = Global::CheatExpBonusCoeff;
			Global::WildBattleFrequency = Global::CheatWildBattleFrequency;
			UEngineDebug::OutPutDebugText("Cheat on!");
		}
	}
}

