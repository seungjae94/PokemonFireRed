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
	__int64 Address = reinterpret_cast<__int64>(&Cond);
	UEngineDebug::OutPutDebugText(GetWorld()->GetName() + " FadeLevelChanger In RegisterPredefinedEvent: " + std::to_string(Address));

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
