#include "ExteriorPalletTownLevel.h"

#include "Global.h"
#include "Warp.h"
#include "DialogueActor.h"
#include "PokemonText.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel() 
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel() 
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정 (디버깅 용도)
	UEventManager::SetPoint(GetName(), "Player", { 70, 142 });
	UEventManager::SetDirection(GetName(), "Player", FTileVector::Down);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting OaksLabWarpSetting = UEventTargetInitialSetting(
		"OaksLabWarp",
		{ 80, 147 }
	);
	AWarp* OaksLabWarp = SpawnEventTrigger<AWarp>(OaksLabWarpSetting);
	OaksLabWarp->SetTargetLevelName("InteriorOaksLabLevel");
	OaksLabWarp->SetTargetPoint({ 6, 12 });
	OaksLabWarp->SetMoveDirection(FTileVector::Up);
	OaksLabWarp->RegisterPredefinedEvent();

	UEventTargetInitialSetting PlayersHouseWarpSetting = UEventTargetInitialSetting(
		"PlayersHouseWarp",
		{ 70, 141 }
	);
	AWarp* PlayersHouseWarp = SpawnEventTrigger<AWarp>(PlayersHouseWarpSetting);
	PlayersHouseWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	PlayersHouseWarp->SetTargetPoint({ 3, 8 });
	PlayersHouseWarp->SetMoveDirection(FTileVector::Up);
	PlayersHouseWarp->RegisterPredefinedEvent();

	UEventTargetInitialSetting RivalsHouseWarpSetting = UEventTargetInitialSetting(
		"RivalsHouseWarp",
		{ 79, 141 }
	);
	AWarp* RivalsHouseWarp = SpawnEventTrigger<AWarp>(RivalsHouseWarpSetting);
	RivalsHouseWarp->SetTargetLevelName("InteriorRivalsHouseLevel");
	RivalsHouseWarp->SetTargetPoint({ 4, 8 });
	RivalsHouseWarp->SetMoveDirection(FTileVector::Up);
	RivalsHouseWarp->RegisterPredefinedEvent();

	UEventTargetInitialSetting FatManSetting = UEventTargetInitialSetting(
		"FatMan",
		{ 80, 151 },
		FTileVector::Down,
		true,
		true,
		true,
		true
	);
	ADialogueActor* FatMan = SpawnEventTrigger<ADialogueActor>(FatManSetting);
	FatMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	FatMan->SetTextColor(EFontColor::Blue);
	FatMan->RegisterPredefinedEvent();
}
