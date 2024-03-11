#include "ExteriorPewterCityLevel.h"

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
	//UEventManager::SetPoint(GetName(), Global::Player, { 10, 10 });			// 회색시티 앞

	MakeForestEntrances();
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

