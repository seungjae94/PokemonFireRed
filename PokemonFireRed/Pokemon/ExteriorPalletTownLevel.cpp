#include "ExteriorPalletTownLevel.h"

#include "Global.h"
#include "Warp.h"
#include "DialogueActor.h"

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
	UEventTargetInitialSetting OaksLabWarpSetting = UEventTargetInitialSetting(
		"OaksLabWarp",
		{ 80, 147 }
	);
	AWarp* OaksLabWarp = SpawnEventTrigger<AWarp>(OaksLabWarpSetting);
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
	OaksLabWarp->SetMoveDirection(FTileVector::Up);

	UEventTargetInitialSetting PlayersHouseWarpSetting = UEventTargetInitialSetting(
		"PlayersHouseWarp",
		{ 70, 141 }
	);
	AWarp* PlayersHouseWarp = SpawnEventTrigger<AWarp>(PlayersHouseWarpSetting);
	PlayersHouseWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	PlayersHouseWarp->SetTargetPoint({ 3, 8 });
	PlayersHouseWarp->SetMoveDirection(FTileVector::Up);

	UEventTargetInitialSetting RivalsHouseWarpSetting = UEventTargetInitialSetting(
		"RivalsHouseWarp",
		{ 79, 141 }
	);
	AWarp* RivalsHouseWarp = SpawnEventTrigger<AWarp>(RivalsHouseWarpSetting);
	RivalsHouseWarp->SetTargetLevelName("InteriorRivalsHouseLevel");
	RivalsHouseWarp->SetTargetPoint({ 4, 8 });
	RivalsHouseWarp->SetMoveDirection(FTileVector::Up);

	UEventTargetInitialSetting FatManSetting = UEventTargetInitialSetting(
		"FatMan",
		{ 80, 151 },
		FTileVector::Down,
		true,
		true,
		true
	);
	ADialogueActor* FatMan = SpawnEventTrigger<ADialogueActor>(FatManSetting);
}
