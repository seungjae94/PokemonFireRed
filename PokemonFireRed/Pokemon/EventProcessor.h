#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventCondition.h"

class AEventTrigger;
class UEventCondition;
enum class EEventTriggerAction;

// 매 프레임마다 이벤트 액터 대신 이벤트를 처리해주는 객체
// 현재 레벨과 무관하게 틱이 돌아간다.
class UEventProcessor
{
	friend UEventManager;
public:
	// constructor destructor
	UEventProcessor();
	~UEventProcessor();

	// delete Function
	UEventProcessor(const UEventProcessor& _Other) = delete;
	UEventProcessor(UEventProcessor&& _Other) noexcept = delete;
	UEventProcessor& operator=(const UEventProcessor& _Other) = delete;
	UEventProcessor& operator=(UEventProcessor&& _Other) noexcept = delete;

	void Register(const UEventCondition& _Condition, Event _Event)
	{
		AllEvents[_Condition].push_back(_Event);
	}
	
	/// <summary>
	/// 이벤트 실행을 시도한다.
	/// 조건을 만족하는 이벤트 그룹이 없을 경우 false를 반환한다.
	bool TryRun(EEventTriggerAction _TriggerAction);

	bool IsRunning() const
	{
		return IsRunningValue;
	}

protected:

private:
	bool IsRunningValue = false;

	UEventCondition CurrentCondition;
	int CurEventIndex = 0;
	std::map<UEventCondition, std::vector<Event>> AllEvents;

	//int EventSetIndex = 0;
	//std::map<int, Event>> AllEvents;

	void Tick(float _DeltaTime);
	void EndRun();
};

