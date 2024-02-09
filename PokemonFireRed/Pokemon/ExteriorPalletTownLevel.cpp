#include "ExteriorPalletTownLevel.h"

#include "Global.h"
#include "Warp.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel() 
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel() 
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// �÷��̾� ���� ��ġ ���� (����� �뵵)
	UEventManager::ChangePoint(GetName(), "Player", { 70, 142 });
	UEventManager::ChangeDirection(GetName(), "Player", FTileVector::Down);

	// �̺�Ʈ Ʈ���� ����
	AWarp* OaksLabWarp = SpawnEventTrigger<AWarp>("OaksLabWarp", {80, 147});
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
	OaksLabWarp->SetMoveDirection(FTileVector::Up);

	AWarp* PlayersHouseWarp = SpawnEventTrigger<AWarp>("PlayersHouseWarp", { 70, 141 });
	PlayersHouseWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	PlayersHouseWarp->SetTargetPoint({ 3, 8 });
	PlayersHouseWarp->SetMoveDirection(FTileVector::Up);

	AWarp* RivalsHouseWarp = SpawnEventTrigger<AWarp>("RivalsHouseWarp", { 79, 141 });
	RivalsHouseWarp->SetTargetLevelName("InteriorRivalsHouseLevel");
	RivalsHouseWarp->SetTargetPoint({ 4, 8 });
	RivalsHouseWarp->SetMoveDirection(FTileVector::Up);
}
