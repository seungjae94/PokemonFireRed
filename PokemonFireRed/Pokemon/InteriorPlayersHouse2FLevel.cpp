#include "InteriorPlayersHouse2FLevel.h"
#include "Warp.h"

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
	UEventManager::ChangePoint(GetName(), "Player", { 5, 6 });
	UEventManager::ChangeDirection(GetName(), "Player", FTileVector::Up);

	// �̺�Ʈ Ʈ���� ����
	AWarp* StairTo1FWarp = SpawnEventTrigger<AWarp>("StairTo1FWarp", {8, 2});
	StairTo1FWarp->SetTargetLevelName("InteriorPlayersHouse1FLevel");
	StairTo1FWarp->SetTargetPoint({9, 2});
	StairTo1FWarp->SetMoveDirection(FTileVector::Left);
}
