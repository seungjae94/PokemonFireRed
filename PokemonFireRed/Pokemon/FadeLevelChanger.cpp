#include "FadeLevelChanger.h"

AFadeLevelChanger::AFadeLevelChanger() 
{
}

AFadeLevelChanger::~AFadeLevelChanger() 
{
}

void AFadeLevelChanger::Update(std::string_view _TargetLevelName, bool _PlayerControl, float _FadeInTime, float _FadeOutTime)
{
	FadeInTime = _FadeInTime;
	FadeOutTime = _FadeOutTime;

	UnregisterEvent();
	TargetLevelName = _TargetLevelName;
	PlayerControl = _PlayerControl;
	RegisterPredefinedEvent();
}

void AFadeLevelChanger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(false)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::ChangeLevel(TargetLevelName)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::End(PlayerControl)
	);
}
