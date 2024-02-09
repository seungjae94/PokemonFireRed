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
		MsgBoxAssert("Ʈ���ſ� �̺�Ʈ�� ������� �ʾҽ��ϴ�.");
		return;
	}

	bool EventEnd = AllEvents[CurEventIndex]();

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
