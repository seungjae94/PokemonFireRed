#include "InteriorPokemonCenterLevel.h"

UInteriorPokemonCenterLevel::UInteriorPokemonCenterLevel() 
{
}

UInteriorPokemonCenterLevel::~UInteriorPokemonCenterLevel() 
{
}

void UInteriorPokemonCenterLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (�����) �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::Player, { 5, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);
}

void UInteriorPokemonCenterLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);

}

