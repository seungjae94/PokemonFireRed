#include "InteriorShopLevel.h"
#include "InteriorDoor.h"

UInteriorShopLevel::UInteriorShopLevel() 
{
}

UInteriorShopLevel::~UInteriorShopLevel() 
{
}

void UInteriorShopLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 4, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeDoor();
}

void UInteriorShopLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);
}

void UInteriorShopLevel::LevelStart(ULevel* _PrevLevel)
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

void UInteriorShopLevel::MakeDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::InteriorDoor);
	Setting.SetPoint({ 4, 8 });

	Door = SpawnEventTrigger<AInteriorDoor>(Setting);
}

void UInteriorShopLevel::SetDoorTargetAsViridianCity()
{
	Door->SetTargetMapName(Global::ExteriorPalletTownLevel);
	Door->SetTargetPoint({ 88, 74 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::ViridianShopDoor);
	Door->SetTargetMapNameText(L"VIRIDIAN CITY");
	Door->RegisterPredefinedEvent();
}
