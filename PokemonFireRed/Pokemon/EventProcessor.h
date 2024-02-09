#pragma once
#include <functional>
#include <map>
#include "EventManager.h"

class AEventTrigger;

// �� �����Ӹ��� �̺�Ʈ ���� ��� �̺�Ʈ�� ó�����ִ� ��ü
// ���� ������ �����ϰ� ƽ�� ���ư���.
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

