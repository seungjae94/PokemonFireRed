#include "InteriorPlayersHouse2FLevel.h"
#include "Warp.h"
#include "EventStream.h"
#include "EventCondition.h"

UInteriorPlayersHouse2FLevel::UInteriorPlayersHouse2FLevel()
{
}

UInteriorPlayersHouse2FLevel::~UInteriorPlayersHouse2FLevel()
{
}

void UInteriorPlayersHouse2FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), "Player", { 5, 6 });
	UEventManager::SetDirection(GetName(), "Player", FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInitialSetting StairTo1FSetting = UEventTargetInitialSetting(
		"StairTo1F",
		{ 8, 2 }
	);
	UEventCondition StairTo1FCond = UEventCondition(EEventTriggerAction::Notice);
	std::string StairTo1FTargetMapName = "InteriorPlayersHouse1FLevel";
	FTileVector StairTo1FTargetPoint = { 9, 2 };
	FTileVector StairTo1FMoveDirection = FTileVector::Left;
	StairTo1FCond.RegisterCheckFunc([StairTo1FMoveDirection]() {
		FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
		return CurPlayerDirection == StairTo1FMoveDirection;
		});

	AEventTrigger* StairTo1F = SpawnEventTrigger<AEventTrigger>(StairTo1FSetting);

	UEventManager::RegisterEvent(StairTo1F, StairTo1FCond,
		ES::Start(true)
		>> ES::MoveWithoutRestriction(GetName(), "Player", { FVector(-0.3f, 0.0f) * Global::F_TILE_SIZE }, 12.0f)
		>> ES::MoveWithoutRestriction(GetName(), "Player", { FVector(-1.0f, 0.25f) * Global::F_TILE_SIZE }, 2.2f)
		>> ES::FadeOut(0.5f)
		>> ES::ChangeLevel(StairTo1FTargetMapName)
		>> ES::ChangePoint(StairTo1FTargetMapName, "Player", StairTo1FTargetPoint)
		>> ES::ChangeDirection(StairTo1FTargetMapName, "Player", StairTo1FMoveDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);
}
