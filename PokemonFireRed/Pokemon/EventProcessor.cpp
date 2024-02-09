#include "EventProcessor.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::Tick(float _DeltaTime)
{
	bool EventEnd = AllEvents[CurEventIndex](_DeltaTime);

	if (true == EventEnd)
	{
		// 더이상 수행할 이벤트가 없는 경우
		if (CurEventIndex + 1 >= AllEvents.size())
		{
			CurEventIndex = 0;
			IsWorkingValue = false;
			return;
		}

		// 다음 이벤트 수행
		CurEventIndex++;
	}
}
