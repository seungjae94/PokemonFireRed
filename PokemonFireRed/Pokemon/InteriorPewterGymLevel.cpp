#include "InteriorPewterGymLevel.h"
#include "InteriorDoor.h"

UInteriorPewterGymLevel::UInteriorPewterGymLevel() 
{
}

UInteriorPewterGymLevel::~UInteriorPewterGymLevel() 
{
}

void UInteriorPewterGymLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	MakePewterCityDoor();
}

void UInteriorPewterGymLevel::MakePewterCityDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("Door");
	Setting.SetPoint({ 6, 15 });

	AInteriorDoor* Door = SpawnEventTrigger<AInteriorDoor>(Setting);
	Door->SetTargetMapName(Global::ExteriorPewterCityLevel);
	Door->SetTargetPoint({ 19, 21 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::PewterGymDoor);
	Door->SetTargetMapNameText(L"PEWTER CITY");
	Door->RegisterPredefinedEvent();
}

