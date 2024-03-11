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

	MakeExitToR2();
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

void UExteriorViridianForestLevel::MakeExitToPewter()
{
}
