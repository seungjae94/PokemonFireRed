#include "InteriorPlayersHouse1FLevel.h"
#include "Stair.h"
#include "InteriorDoor.h"
#include "DialogueActor.h"
#include "PokemonText.h"
#include "EventCondition.h"
#include "EventStream.h"

UInteriorPlayersHouse1FLevel::UInteriorPlayersHouse1FLevel()
{
}

UInteriorPlayersHouse1FLevel::~UInteriorPlayersHouse1FLevel()
{
}

void UInteriorPlayersHouse1FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 4, 3 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo2FSetting = UEventTargetInitialSetting(
		"StairTo2F",
		{ 10, 2 }
	);

	AStair* StairTo2F = SpawnEventTrigger<AStair>(StairTo2FSetting);
	StairTo2F->SetTargetMapName(Global::InteriorPlayersHouse2FLevel);
	StairTo2F->SetTargetPoint({ 9, 2 });
	StairTo2F->SetMoveDirection(FTileVector::Right);
	StairTo2F->SetPath({ FVector(0.1f, 0.0f) * Global::FloatTileSize }, { FVector(1.0f, -0.5f) * Global::FloatTileSize });
	StairTo2F->RegisterPredefinedEvent();

	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownDoor",
		{ 3, 9 }
	);
	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownWarpSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 70, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->RegisterPredefinedEvent();

	UEventTargetInitialSetting PlayersMomSetting = UEventTargetInitialSetting(
		"PlayersMom",
		{ 7, 4 },
		FTileVector::Left,
		true,
		true,
		false,
		true
	);
	ADialogueActor* PlayersMom = SpawnEventTrigger<ADialogueActor>(PlayersMomSetting);
	PlayersMom->SetDialogue({
		LR"(MOM: ...Right.
All boys leave home someday.)",
LR"(It said so on TV.)",
LR"(Oh, yes. PROF. OAK, next door, was
looking for you.)"
		});
	PlayersMom->SetTextColor(EFontColor::Red);
	PlayersMom->RegisterPredefinedEvent();
}
