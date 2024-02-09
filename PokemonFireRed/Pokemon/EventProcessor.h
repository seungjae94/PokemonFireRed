#pragma once
#include <functional>
#include <map>
#include "EventManager.h"

class AEventTrigger;

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

	void Register(Event _Event)
	{
		int Size = AllEvents.size();

		AllEvents.push_back(_Event);
	}
	
	void Work()
	{
		IsWorkingValue = true;
	}

	bool IsWorking()
	{
		return IsWorkingValue;
	}

protected:

private:
	bool IsWorkingValue = false;

	int CurEventIndex = 0;
	std::vector<Event> AllEvents;
	//int EventSetIndex = 0;
	//std::map<int, Event>> AllEvents;

	void Tick(float _DeltaTime);
};

