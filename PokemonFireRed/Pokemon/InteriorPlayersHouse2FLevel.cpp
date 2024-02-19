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
	UEventManager::SetPoint(GetName(), Global::PlayerName, { 5, 6 });
	UEventManager::SetDirection(GetName(), Global::PlayerName, FTileVector::Up);

	// �̺�Ʈ Ʈ���� ����
	UEventTargetInit StairTo1FSetting; 
	StairTo1FSetting.SetName("StairTo1F");
	StairTo1FSetting.SetPoint({ 8, 2 });

	AStair* StairTo1F = SpawnEventTrigger<AStair>(StairTo1FSetting);
	StairTo1F->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	StairTo1F->SetTargetPoint({ 9, 2 });
	StairTo1F->SetMoveDirection(FTileVector::Left);
	StairTo1F->SetFirstPath({FVector(-0.5f, 0.125f) * Global::FloatTileSize, FVector(-0.5f, 0.125f) * Global::FloatTileSize });
	StairTo1F->SetSecondPath({FVector(-0.5f, 0.25f) * Global::FloatTileSize, FVector(-0.5f, 0.25f) * Global::FloatTileSize });
	StairTo1F->RegisterPredefinedEvent();
}
