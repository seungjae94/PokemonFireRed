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
	ABlackScreen* BlackScreen = SpawnUIElement<ABlackScreen>("BlackScreen");
	AWhiteScreen* WhiteScreen = SpawnUIElement<AWhiteScreen>("WhiteScreen");
	ACurtainScreen* CurtainScreen = SpawnUIElement<ACurtainScreen>("CurtainScreen");

	// 레벨 체인저 생성
	FadeLevelChanger = SpawnEventTrigger<AFadeLevelChanger>("FadeLevelChanger");
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

