#include "InteriorPlayersHouse2FLevel.h"
#include "Stair.h"
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

	// �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::PLAYER_NAME, { 5, 6 });
	UEventManager::SetDirection(GetName(), Global::PLAYER_NAME, FTileVector::Up);

	// �̺�Ʈ Ʈ���� ����
	UEventTargetInitialSetting StairTo1FSetting = UEventTargetInitialSetting(
		"StairTo1F",
		{ 8, 2 }
	);

	AStair* StairTo1F = SpawnEventTrigger<AStair>(StairTo1FSetting);
	StairTo1F->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	StairTo1F->SetTargetPoint({ 9, 2 });
	StairTo1F->SetMoveDirection(FTileVector::Left);
	StairTo1F->SetPath({ FVector(-0.3f, 0.0f) * Global::FloatTileSize }, { FVector(-1.0f, 0.25f) * Global::FloatTileSize });
	StairTo1F->RegisterPredefinedEvent();
}
