#include "ExteriorPewterCityLevel.h"
#include "ExteriorDoor.h"
#include "ClosedDoor.h"
#include "EventStream.h"

UExteriorPewterCityLevel::UExteriorPewterCityLevel()
{
}

UExteriorPewterCityLevel::~UExteriorPewterCityLevel()
{
}

void UExteriorPewterCityLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	//UEventManager::SetPoint(GetName(), Global::Player, { 19, 21 });				// 회색 체육관 앞
	UEventManager::SetPoint(GetName(), Global::PlayerCharacter, { 21, 57 });				// 상록숲 앞
	UEventManager::SetDirection(GetName(), Global::PlayerCharacter, FTileVector::Up);

	MakeForestEntrances();
	MakePokemonCenterDoor();
	MakeGymDoor();
	MakeShopClosedDoor();
	MakePrivateHouse1ClosedDoor();
	MakePrivateHouse2ClosedDoor();
	MakeMuseumClosedDoor();
	MakeAnimatedTiles();

	MakePewterToRoute2AreaChanger();
	MakeRoute2ToPewterAreaChanger();
}

void UExteriorPewterCityLevel::MakeForestEntrances()
{
	std::function<bool()> Cond = []() {
		const APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);
		return Player->GetDirection() == FTileVector::Down;
	};

	for (int i = 0; i < 2; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ForestEntrance" + std::to_string(i));
		Setting.SetPoint({ 21 + i, 58 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		Trigger->RegisterEvent(
			EEventTriggerAction::ArrowClick,
			Cond,
			ES::Start(true)
			>> ES::FadeOutBgm(0.75f)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorViridianForestLevel)
			>> ES::ChangePoint(Global::ExteriorViridianForestLevel, EN::PlayerCharacter, { 11, 10 })
			>> ES::ChangeDirection(Global::ExteriorViridianForestLevel, Global::PlayerCharacter, FTileVector::Down)
			>> ES::CameraFocus(Global::PlayerCharacter)
			>> ES::ShowMapName(L"VIRIDIAN FOREST")
			>> ES::PlayBgm(RN::BgmViridianForest)
			>> ES::FadeInBgm(0.75f)
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

void UExteriorPewterCityLevel::MakePewterToRoute2AreaChanger()
{
	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("PewterToRoute2AreaChanger" + std::to_string(i));
		Setting.SetPoint({ 24 + i, 44 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn,
			IsPlayerNotInRoute2,
			ES::Start(false)
			>> ES::ChangeArea("ROUTE 2", RN::BgmRoute2)
			>> ES::ShowMapName(L"ROUTE 2")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmRoute2)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

void UExteriorPewterCityLevel::MakeRoute2ToPewterAreaChanger()
{
	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("Route2ToPewterAreaChanger" + std::to_string(i));
		Setting.SetPoint({ 24 + i, 43 });
		Setting.SetHeight(1);

		AEventTrigger* Changer = SpawnEventTrigger<AEventTrigger>(Setting);

		Changer->RegisterEvent(
			EEventTriggerAction::StepOn,
			IsPlayerNotInPewterCity,
			ES::Start(false)
			>> ES::ChangeArea("PEWTER CITY", RN::BgmPewterCity)
			>> ES::ShowMapName(L"PEWTER CITY")
			>> ES::FadeOutBgm(0.25f)
			>> ES::Wait(0.25f)
			>> ES::PlayBgm(RN::BgmPewterCity)
			>> ES::FadeInBgm(0.25f)
			>> ES::End(false));
	}
}

std::string UExteriorPewterCityLevel::GetAreaNameStatic()
{
	APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);
	UExteriorPewterCityLevel* CurLevel = dynamic_cast<UExteriorPewterCityLevel*>(Player->GetWorld());
	if (nullptr == CurLevel)
	{
		MsgBoxAssert("UExteriorPewterCityLevel::GetAreaNameStatic 함수를 UExteriorPewterCityLevel 외부에서 호출했습니다.");
		return "";
	}

	return CurLevel->GetAreaName();
}

bool UExteriorPewterCityLevel::IsPlayerNotInPewterCity()
{
	return GetAreaNameStatic() != "PEWTER CITY";
}

bool UExteriorPewterCityLevel::IsPlayerNotInRoute2()
{
	return GetAreaNameStatic() != "ROUTE 2";
}