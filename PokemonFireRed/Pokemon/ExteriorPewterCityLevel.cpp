#include "ExteriorPewterCityLevel.h"
#include "ExteriorDoor.h"
#include "ClosedDoor.h"

UExteriorPewterCityLevel::UExteriorPewterCityLevel()
{
}

UExteriorPewterCityLevel::~UExteriorPewterCityLevel()
{
}

void UExteriorPewterCityLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (�����) �÷��̾� ���� ��ġ ����
	//UEventManager::SetPoint(GetName(), Global::Player, { 19, 21 });				// ȸ�� ü���� ��
	UEventManager::SetPoint(GetName(), Global::Player, { 21, 57 });				// ��Ͻ� ��
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeForestEntrances();
	MakePokemonCenterDoor();
	MakeGymDoor();
	MakeShopClosedDoor();
	MakePrivateHouse1ClosedDoor();
	MakePrivateHouse2ClosedDoor();
	MakeMuseumClosedDoor();
	MakeAnimatedTiles();
}

void UExteriorPewterCityLevel::MakeForestEntrances()
{
	UEventCondition Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc([]() {
		const APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(EN::Player);
		return Player->GetDirection() == FTileVector::Down;
		});

	for (int i = 0; i < 2; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ForestEntrance" + std::to_string(i));
		Setting.SetPoint({ 21 + i, 58 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		UEventManager::RegisterEvent(Trigger, Cond,
			ES::Start(true)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorViridianForestLevel)
			>> ES::ChangePoint(Global::ExteriorViridianForestLevel, EN::Player, { 11, 10 })
			>> ES::ChangeDirection(Global::ExteriorViridianForestLevel, Global::Player, FTileVector::Down)
			>> ES::CameraFocus(Global::Player)
			>> ES::ShowMapName(L"VIRIDIAN FOREST")
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}

void UExteriorPewterCityLevel::MakePokemonCenterDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::PewterPokemonCenterDoor);
	Setting.SetPoint({ 21, 29 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::SlideDoor);
	Setting.SetHeight(1);

	AExteriorDoor* Door = SpawnEventTrigger<AExteriorDoor>(Setting);
	Door->SetTargetMapName(Global::InteriorPokemonCenterLevel);
	Door->SetTargetPoint({ 7, 8 });
	Door->SetMoveDirection(FTileVector::Up);
	Door->SetTargetBgm(RN::BgmPokemonCenter);
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakeGymDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::PewterGymDoor);
	Setting.SetPoint({ 19, 20 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName(RN::GymDoor);
	Setting.SetHeight(1);

	AExteriorDoor* Door = SpawnEventTrigger<AExteriorDoor>(Setting);
	Door->SetTargetMapName(Global::InteriorPewterGymLevel);
	Door->SetTargetPoint({ 6, 14 });
	Door->SetMoveDirection(FTileVector::Up);
	Door->SetTargetBgm(RN::BgmGym);
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakeAnimatedTiles()
{
	DrawFlowers({
		{31, 30}, {32, 30}, {33, 30}, {34, 30}, {36, 30}, {37, 30}, {38, 30}, {39, 30},
		{31, 32}, {32, 32}, {33, 32}, {34, 32}, {36, 32}, {37, 32}, {38, 32}, {39, 32}
		});
}

void UExteriorPewterCityLevel::MakeShopClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("ShopClosedDoor");
	Setting.SetPoint({ 32, 22 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakePrivateHouse1ClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("PrivateHouse1ClosedDoor");
	Setting.SetPoint({ 37, 15 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakePrivateHouse2ClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("PrivateHouse2ClosedDoor");
	Setting.SetPoint({ 13, 34 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakeMuseumClosedDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("MuseumClosedDoor");
	Setting.SetPoint({ 21, 10 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetHeight(1);
	Setting.SetCollidable(true);

	AClosedDoor* Door = SpawnEventTrigger<AClosedDoor>(Setting);
	Door->RegisterPredefinedEvent();
}