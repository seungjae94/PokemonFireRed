#include "InteriorPlayersHouse1FLevel.h"
#include "Warp.h"
#include "DialogueActor.h"
#include "PokemonText.h"

UInteriorPlayersHouse1FLevel::UInteriorPlayersHouse1FLevel()
{
}

UInteriorPlayersHouse1FLevel::~UInteriorPlayersHouse1FLevel()
{
}

void UInteriorPlayersHouse1FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo2FWarpSetting = UEventTargetInitialSetting(
		"StairTo2FWarp",
		{ 10, 2 }
	);
	AWarp* StairTo2FWarp = SpawnEventTrigger<AWarp>(StairTo2FWarpSetting);
	StairTo2FWarp->SetTargetLevelName("InteriorPlayersHouse2FLevel");
	StairTo2FWarp->SetTargetPoint({ 9, 2 });
	StairTo2FWarp->SetMoveDirection(FTileVector::Right);

	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownWarp",
		{ 3, 9 }
	);
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 70, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);

	UEventTargetInitialSetting PlayersMomSetting = UEventTargetInitialSetting(
		"PlayersMom",
		{ 7, 4 },
		FTileVector::Left,
		true,
		false,
		true
	);
	ADialogueActor* PlayersMom = SpawnEventTrigger<ADialogueActor>(PlayersMomSetting);
	PlayersMom->SetDialogue({
		LR"(MOM: ...Right.
All boys leave home someday.)"
		});
	PlayersMom->SetTextColor(EFontColor::Red);
}
