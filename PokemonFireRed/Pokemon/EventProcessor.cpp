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
		// ���̻� ������ �̺�Ʈ�� ���� ���
		if (CurEventIndex + 1 >= AllEvents.size())
		{
			CurEventIndex = 0;
			IsWorkingValue = false;
			return;
		}

		// ���� �̺�Ʈ ����
		CurEventIndex++;
	}
}
