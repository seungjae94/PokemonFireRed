#include "PokemonLevel.h"
#include "EventManager.h"
#include "BlackScreen.h"
#include "WhiteScreen.h"
#include "CurtainScreen.h"

UPokemonLevel::UPokemonLevel()
{
}

UPokemonLevel::~UPokemonLevel()
{
}

void UPokemonLevel::BeginPlay()
{
	// Ÿ��Ʋ ����, Ʃ�丮�� ������ �����ϴ� ���� UI ������Ʈ ����
	SpawnMapLevelCanvas<ABlackScreen>(Global::BlackScreen);
	SpawnMapLevelCanvas<AWhiteScreen>(Global::WhiteScreen);
	SpawnMapLevelCanvas<ACurtainScreen>(Global::CurtainScreen);

	// ���� ���� Ʈ���� ����
	SpawnEventTrigger<AFadeLevelChanger>(Global::FadeLevelChanger);
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

