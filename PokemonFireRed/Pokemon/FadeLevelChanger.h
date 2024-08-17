#pragma once
#include "EventTrigger.h"
#include "EventEnums.h"

class AFadeLevelChanger : public AEventTrigger
{
public:
	// constructor destructor
	AFadeLevelChanger();
	~AFadeLevelChanger();

	// delete Function
	AFadeLevelChanger(const AFadeLevelChanger& _Other) = delete;
	AFadeLevelChanger(AFadeLevelChanger&& _Other) noexcept = delete;
	AFadeLevelChanger& operator=(const AFadeLevelChanger& _Other) = delete;
	AFadeLevelChanger& operator=(AFadeLevelChanger&& _Other) noexcept = delete;

	void Trigger(std::string_view _TargetLevelName, bool _PlayerControl, float _FadeInTime, float _FadeOutTime, bool _IsFadeBgm, std::string_view _NewBgm)
	{
		Update(_TargetLevelName, _PlayerControl, _FadeInTime, _FadeOutTime, _IsFadeBgm, _NewBgm);
		TriggerEvent(EEventTriggerAction::Direct);
	}

protected:
private:
	std::string TargetLevelName;
	float FadeOutTime = 1.0f;
	float FadeInTime = 1.0f;
	bool PlayerControl = true;

	void Update(std::string_view _Name, bool _PlayerControl, float _FadeInTime, float _FadeOutTime, bool _IsFadeBgm = false, std::string_view _NewBgm = "");
	void RegisterPredefinedEvent() override;
	void RegisterPredefinedEventWithBgm(std::string_view _NewBgm);
};

