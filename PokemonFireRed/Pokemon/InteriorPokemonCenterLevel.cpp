#include "InteriorPokemonCenterLevel.h"
#include "InteriorDoor.h"

UInteriorPokemonCenterLevel::UInteriorPokemonCenterLevel() 
{
}

UInteriorPokemonCenterLevel::~UInteriorPokemonCenterLevel() 
{
}

void UInteriorPokemonCenterLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 5, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeDoor();
	MakeNurse();
}

void UInteriorPokemonCenterLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);

}

void UInteriorPokemonCenterLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);
	
	// 맵 레벨에서 직접 포켓몬 센터에 들어온 경우
	if (nullptr != MapLevel)
	{
		if (MapLevel->GetName() == UEngineString::ToUpper(Global::ExteriorPalletTownLevel))
		{
			SetDoorTargetAsViridianCity();
		}
		return;
	}
	
	// 배틀에서 패배해서 포켓몬 센터로 온 경우
	SetDoorTargetAsViridianCity(); // 임시로 상록 시티로 돌아오도록 설정
}

void UInteriorPokemonCenterLevel::MakeDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::InteriorDoor);
	Setting.SetPoint({ 7, 9 });

	Door = SpawnEventTrigger<AInteriorDoor>(Setting);
}

void UInteriorPokemonCenterLevel::MakeNurse()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::Nurse);
	Setting.SetPoint({7, 2});
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetImageNameAuto();

	AEventTarget* Nurse = SpawnEventTarget<AEventTarget>(Setting);
}

void UInteriorPokemonCenterLevel::SetDoorTargetAsViridianCity()
{
	Door->SetTargetMapName(Global::ExteriorPalletTownLevel);
	Door->SetTargetPoint({ 78, 81 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::ViridianPokemonCenterDoor);
	Door->SetTargetMapNameText(L"VIRIDIAN CITY");
	Door->RegisterPredefinedEvent();
}

