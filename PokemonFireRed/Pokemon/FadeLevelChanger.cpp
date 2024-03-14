#include "FadeLevelChanger.h"

AFadeLevelChanger::AFadeLevelChanger() 
{
}

AFadeLevelChanger::~AFadeLevelChanger() 
{
}

void AFadeLevelChanger::Update(std::string_view _TargetLevelName, bool _PlayerControl, float _FadeInTime, float _FadeOutTime, bool _IsFadeBgm, std::string_view _NewBgm)
{
	FadeInTime = _FadeInTime;
	FadeOutTime = _FadeOutTime;

	UnregisterEvent();
	TargetLevelName = _TargetLevelName;
	PlayerControl = _PlayerControl;

	if (false == _IsFadeBgm)
	{
		RegisterPredefinedEvent();
	}
	else
	{
		RegisterPredefinedEventWithBgm(_NewBgm);
	}
}

void AFadeLevelChanger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(false)
		>> ES::Achieve(EAchievement::Fading)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::ChangeLevel(TargetLevelName)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::Unachieve(EAchievement::Fading)
		>> ES::End(PlayerControl)
	);
}

void AFadeLevelChanger::RegisterPredefinedEventWithBgm(std::string_view _NewBgm)
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(false)
		>> ES::Achieve(EAchievement::Fading)
		>> ES::FadeOut(FadeOutTime)
		>> ES::FadeOutBgm(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::ChangeLevel(TargetLevelName)
		>> ES::PlayBgm(_NewBgm)
		>> ES::FadeIn(FadeInTime)
		>> ES::FadeInBgm(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::Unachieve(EAchievement::Fading)
		>> ES::End(PlayerControl)
	);
}
