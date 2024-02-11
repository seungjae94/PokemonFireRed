#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

bool UEventProcessor::TryRun(EEventTriggerAction _TriggerAction)
{
	for (std::pair<const UEventCondition, std::vector<Event>>& Pair : AllEvents)
	{
		const UEventCondition& Condition = Pair.first;
		
		bool CheckResult = Condition.Check(_TriggerAction);
		if (true == CheckResult)
		{
			CurrentCondition = Condition;
			IsRunningValue = true;
			return true;
		}
	}

	return false;
}

void UEventProcessor::EndRun()
{
	CurEventIndex = 0;
	IsRunningValue = false;
}

void UEventProcessor::Tick(float _DeltaTime)
{
	if (CurEventIndex >= AllEvents[CurrentCondition].size())
	{
		MsgBoxAssert("트리거에 이벤트를 등록하지 않았습니다.");
		return;
	}

	bool EventEnd = AllEvents[CurrentCondition][CurEventIndex]();

	if (true == EventEnd)
	{
		// 더이상 수행할 이벤트가 없는 경우
		if (CurEventIndex + 1 >= AllEvents[CurrentCondition].size())
		{
			EndRun();
			return;
		}

		// 다음 이벤트 수행
		CurEventIndex++;
	}
}
