#include "ExteriorViridianForestLevel.h"

UExteriorViridianForestLevel::UExteriorViridianForestLevel() 
{
}

UExteriorViridianForestLevel::~UExteriorViridianForestLevel() 
{
}

void UExteriorViridianForestLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 10, 10 });			// 회색시티 앞

	MakeExitToR2();
	MakeExitsToPewter();
}

void UExteriorViridianForestLevel::MakeExitToR2()
{
	UEventCondition Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc([](){
		const APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(EN::Player);
		return Player->GetDirection() == FTileVector::Down;
	});

	for (int i = 0; i < 3; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ExitToR2Target" + std::to_string(i));
		Setting.SetPoint({ 34 + i, 63 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		UEventManager::RegisterEvent(Trigger, Cond,
			ES::Start(true)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorPalletTownLevel)
			>> ES::ChangePoint(Global::ExteriorPalletTownLevel, EN::Player, { 69, 26 })
			>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, Global::Player, FTileVector::Down)
			>> ES::CameraFocus(Global::Player)
			>> ES::ShowMapName(L"VIRIDIAN CITY")
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}

void UExteriorViridianForestLevel::MakeExitsToPewter()
{
	UEventCondition Cond = UEventCondition(EEventTriggerAction::StepOn);

	for (int i = 0; i < 3; ++i)
	{
		UEventTargetSetting Setting;
		Setting.SetName("ExitToPewterTarget" + std::to_string(i));
		Setting.SetPoint({ 10 + i, 9 });
		Setting.SetHeight(1);

		AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(Setting);

		UEventManager::RegisterEvent(Trigger, Cond,
			ES::Start(true)
			>> ES::FadeOut(0.75f)
			>> ES::Wait(0.75f)
			>> ES::ChangeLevel(Global::ExteriorPewterCityLevel)
			>> ES::ChangePoint(Global::ExteriorPewterCityLevel, EN::Player, { 22, 57 })
			>> ES::ChangeDirection(Global::ExteriorPewterCityLevel, Global::Player, FTileVector::Up)
			>> ES::CameraFocus(Global::Player)
			>> ES::ShowMapName(L"PEWTER CITY")
			>> ES::FadeIn(0.75f)
			>> ES::Wait(0.75f)
			>> ES::End(true)
		);
	}
}
