#pragma once
#include "EventTrigger.h"

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

	void ChangeLevel(std::string_view _Name, float _FadeInTime, float _FadeOutTime)
	{
		SetTargetLevelName(_Name, _FadeInTime, _FadeOutTime);
		UEventManager::TriggerEvent(this, EEventTriggerAction::Direct);
	}

protected:
private:
	UEventCondition Cond;
	std::string TargetLevelName;
	float FadeOutTime = 1.0f;
	float FadeInTime = 1.0f;

	void SetTargetLevelName(std::string_view _Name, float _FadeInTime, float _FadeOutTime)
	{
		FadeInTime = _FadeInTime;
		FadeOutTime = _FadeOutTime;

		if (TargetLevelName != _Name)
		{
			UnregisterEvent();
		}

		TargetLevelName = _Name;
		RegisterPredefinedEvent();
	}
	void RegisterPredefinedEvent() override;
	void UnregisterEvent();


};

