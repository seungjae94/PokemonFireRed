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

	// (�����) �÷��̾� ���� ��ġ ����
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

	// �� �������� ���� ���ϸ� ���Ϳ� ���� ���
	if (nullptr != MapLevel)
	{
		if (MapLevel->GetName() == UEngineString::ToUpper(Global::ExteriorPalletTownLevel))
		{
			SetDoorTargetAsViridianCity();
		}
		return;
	}

	// ��Ʋ���� �й��ؼ� ���ϸ� ���ͷ� �� ���
	SetDoorTargetAsViridianCity(); // �ӽ÷� ��� ��Ƽ�� ���ƿ����� ����
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
