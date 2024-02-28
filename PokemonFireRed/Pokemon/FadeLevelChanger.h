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

	void Trigger(std::string_view _TargetLevelName, float _FadeInTime, float _FadeOutTime)
	{
		Update(_TargetLevelName, _FadeInTime, _FadeOutTime);
		UEventManager::TriggerEvent(this, EEventTriggerAction::Direct);
	}

protected:
private:
	std::string TargetLevelName;
	float FadeOutTime = 1.0f;
	float FadeInTime = 1.0f;

	void Update(std::string_view _Name, float _FadeInTime, float _FadeOutTime);
	void RegisterPredefinedEvent() override;
};

