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
	ABlackScreen* BlackScreen = SpawnUIElement<ABlackScreen>("BlackScreen");
	AWhiteScreen* WhiteScreen = SpawnUIElement<AWhiteScreen>("WhiteScreen");
	ACurtainScreen* CurtainScreen = SpawnUIElement<ACurtainScreen>("CurtainScreen");

	// ���� ü���� ����
	FadeLevelChanger = SpawnEventTrigger<AFadeLevelChanger>("FadeLevelChanger");
}

void UPokemonLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
	UEventManager::Tick(_DeltaTime);
}

