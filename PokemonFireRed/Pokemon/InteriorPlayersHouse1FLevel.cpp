#include "InteriorPlayersHouse1FLevel.h"
#include "Warp.h"
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

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo2FSetting = UEventTargetInitialSetting(
		"StairTo2FWarp",
		{ 10, 2 }
	);
	// AWarp가 아닌 AEventTrigger로 해보기... -> 계단 클래스로 만들기...
	/*AWarp* StairTo2FWarp = SpawnEventTrigger<AWarp>(StairTo2FWarpSetting);
	StairTo2FWarp->SetTargetLevelName("InteriorPlayersHouse2FLevel");
	StairTo2FWarp->SetTargetPoint({ 9, 2 });
	StairTo2FWarp->SetMoveDirection(FTileVector::Right);
	StairTo2FWarp->RegisterPredefinedEvent();*/

	UEventCondition StairTo2FCond = UEventCondition(EEventTriggerAction::Notice);
	std::string StairTo2FTargetMapName = "InteriorPlayersHouse2FLevel";
	FTileVector StairTo2FTargetPoint = {9, 2};
	FTileVector StairTo2FMoveDirection = FTileVector::Right;
	StairTo2FCond.RegisterCheckFunc([StairTo2FMoveDirection]() {
		FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
		return CurPlayerDirection == StairTo2FMoveDirection;
	});

	AEventTrigger* StairTo2F = SpawnEventTrigger<AEventTrigger>(StairTo2FSetting);

	UEventManager::RegisterEvent(StairTo2F, StairTo2FCond,
		ES::Start(true)
		>> ES::MoveWithoutRestriction(GetName(), "Player", { FVector(1.0f, -0.5f) * Global::F_TILE_SIZE }, 1.2f)
		>> ES::FadeOut(0.5f)
		>> ES::ChangeLevel(StairTo2FTargetMapName)
		>> ES::ChangePoint(StairTo2FTargetMapName, "Player", StairTo2FTargetPoint)
		>> ES::ChangeDirection(StairTo2FTargetMapName, "Player", StairTo2FMoveDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownWarp",
		{ 3, 9 }
	);
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName("ExteriorPalletTownLevel");
	PalletTownWarp->SetTargetPoint({ 70, 142 });
	PalletTownWarp->SetMoveDirection(FTileVector::Down);
	PalletTownWarp->RegisterPredefinedEvent();

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
