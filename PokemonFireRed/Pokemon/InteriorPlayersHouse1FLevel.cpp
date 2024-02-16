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

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 4, 3 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo2FSetting = UEventTargetInitialSetting(
		"StairTo2FWarp",
		{ 10, 2 }
	);
	UEventCondition StairTo2FCond = UEventCondition(EEventTriggerAction::Notice);
	std::string StairTo2FTargetMapName = Global::InteriorPlayersHouse2FLevel;
	FTileVector StairTo2FTargetPoint = {9, 2};
	FTileVector StairTo2FMoveDirection = FTileVector::Right;
	StairTo2FCond.RegisterCheckFunc([StairTo2FMoveDirection]() {
		FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
		return CurPlayerDirection == StairTo2FMoveDirection;
	});

	AEventTrigger* StairTo2F = SpawnEventTrigger<AEventTrigger>(StairTo2FSetting);

	UEventManager::RegisterEvent(StairTo2F, StairTo2FCond,
		ES::Start(true)
		>> ES::MoveWithoutRestriction(GetName(), Global::PLAYER_NAME, { FVector(0.1f, 0.0f) * Global::FloatTileSize }, 6.0f)
		>> ES::MoveWithoutRestriction(GetName(), Global::PLAYER_NAME, { FVector(1.0f, -0.5f) * Global::FloatTileSize }, 2.2f)
		>> ES::FadeOut(0.5f)
		>> ES::ChangeLevel(StairTo2FTargetMapName)
		>> ES::ChangePoint(StairTo2FTargetMapName, Global::PLAYER_NAME, StairTo2FTargetPoint)
		>> ES::ChangeDirection(StairTo2FTargetMapName, Global::PLAYER_NAME, StairTo2FMoveDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	UEventTargetInitialSetting PalletTownWarpSetting = UEventTargetInitialSetting(
		"PalletTownWarp",
		{ 3, 9 }
	);
	AWarp* PalletTownWarp = SpawnEventTrigger<AWarp>(PalletTownWarpSetting);
	PalletTownWarp->SetTargetLevelName(Global::ExteriorPalletTownLevel);
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
