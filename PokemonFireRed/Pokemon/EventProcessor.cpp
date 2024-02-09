#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::Tick(float _DeltaTime)
{
	if (CurEventIndex >= AllEvents.size())
	{
		MsgBoxAssert("트리거에 이벤트를 등록하지 않았습니다.");
		return;
	}

	bool EventEnd = AllEvents[CurEventIndex]();

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
