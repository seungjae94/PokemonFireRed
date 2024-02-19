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
	UEventTargetInit StairTo2FSetting;
	StairTo2FSetting.SetName("StairTo2F");
	StairTo2FSetting.SetPoint({ 10, 2 });

	AStair* StairTo2F = SpawnEventTrigger<AStair>(StairTo2FSetting);
	StairTo2F->SetTargetMapName(Global::InteriorPlayersHouse2FLevel);
	StairTo2F->SetTargetPoint({ 9, 2 });
	StairTo2F->SetMoveDirection(FTileVector::Right);
	StairTo2F->SetFirstPath({ FVector(0.5f, -0.25f) * Global::FloatTileSize, FVector(0.5f, -0.25f) * Global::FloatTileSize });
	StairTo2F->SetSecondPath({ FVector(0.5f, -0.125f) * Global::FloatTileSize, FVector(0.5f, -0.125f) * Global::FloatTileSize });
	StairTo2F->RegisterPredefinedEvent();


	UEventTargetInit PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 3, 9 });

	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 70, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("PlayersHouseDoor");
	PalletTownDoor->RegisterPredefinedEvent();


	UEventTargetInit PlayersMomSetting;
	PlayersMomSetting.SetName("PlayersMom");
	PlayersMomSetting.SetPoint({ 7, 4 });
	PlayersMomSetting.SetDirection(FTileVector::Left);
	PlayersMomSetting.SetCollidable(true);
	PlayersMomSetting.SetRotatable(true);
	PlayersMomSetting.SetWalkable(false);
	PlayersMomSetting.SetImageNameAuto();

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
