#include "PokemonLevel.h"
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

	// ���� ���� Ʈ���� ����
	SpawnEventTrigger<AFadeLevelChanger>(Global::FadeLevelChanger);
}

void UPokemonLevel::LevelStart(ULevel* _PrevLevel)
{
	FadeCanvas->Clear();
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

