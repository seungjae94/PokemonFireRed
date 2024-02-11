#pragma once
#include <functional>
#include <map>
#include "EventManager.h"
#include "EventCondition.h"

class AEventTrigger;
class UEventCondition;
enum class EEventTriggerAction;

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

	void Register(const UEventCondition& _Condition, Event _Event)
	{
		AllEvents[_Condition].push_back(_Event);
	}
	
	/// <summary>
	/// �̺�Ʈ ������ �õ��Ѵ�.
	/// ������ �����ϴ� �̺�Ʈ �׷��� ���� ��� false�� ��ȯ�Ѵ�.
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

