#include "InteriorShopLevel.h"
#include "InteriorDoor.h"

InteriorShopLevel::InteriorShopLevel() 
{
}

InteriorShopLevel::~InteriorShopLevel() 
{
}

void InteriorShopLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (�����) �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::Player, { 4, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeDoor();
}

void InteriorShopLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);
}

void InteriorShopLevel::LevelStart(ULevel* _PrevLevel)
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

void InteriorShopLevel::MakeDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::InteriorDoor);
	Setting.SetPoint({ 4, 8 });

	Door = SpawnEventTrigger<AInteriorDoor>(Setting);
}

void InteriorShopLevel::SetDoorTargetAsViridianCity()
{
	Door->SetTargetMapName(Global::ExteriorPalletTownLevel);
	Door->SetTargetPoint({ 88, 74 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::ViridianShopDoor);
	Door->SetTargetMapNameText(L"VIRIDIAN CITY");
	Door->RegisterPredefinedEvent();
}
