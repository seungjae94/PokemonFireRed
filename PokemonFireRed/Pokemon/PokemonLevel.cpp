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
	// 타이틀 레벨, 튜토리얼 레벨을 포함하는 공용 UI 엘리먼트 생성
	SpawnMapLevelCanvas<ABlackScreen>(Global::BlackScreen);
	SpawnMapLevelCanvas<AWhiteScreen>(Global::WhiteScreen);
	SpawnMapLevelCanvas<ACurtainScreen>(Global::CurtainScreen);

	// 게임 공용 트리거 생성
	SpawnEventTrigger<AFadeLevelChanger>(Global::FadeLevelChanger);
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

