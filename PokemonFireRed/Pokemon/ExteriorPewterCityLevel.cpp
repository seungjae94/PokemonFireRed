#include "ExteriorPewterCityLevel.h"
#include "ExteriorDoor.h"

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
	UEventManager::SetPoint(GetName(), Global::Player, { 21, 31 });

	MakeForestEntrances();
	MakePokemonCenterDoor();
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
	Door->RegisterPredefinedEvent();
}

void UExteriorPewterCityLevel::MakeAnimatedTiles()
{
	DrawFlowers({
		{31, 30}, {32, 30}, {33, 30}, {34, 30}, {36, 30}, {37, 30}, {38, 30}, {39, 30},
		{31, 32}, {32, 32}, {33, 32}, {34, 32}, {36, 32}, {37, 32}, {38, 32}, {39, 32}
	});
}
