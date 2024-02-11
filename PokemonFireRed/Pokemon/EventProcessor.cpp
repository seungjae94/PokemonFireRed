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
		MsgBoxAssert("Ʈ���ſ� �̺�Ʈ�� ������� �ʾҽ��ϴ�.");
		return;
	}

	bool EventEnd = AllEvents[CurrentCondition][CurEventIndex]();

	if (true == EventEnd)
	{
		// ���̻� ������ �̺�Ʈ�� ���� ���
		if (CurEventIndex + 1 >= AllEvents[CurrentCondition].size())
		{
			EndRun();
			return;
		}

		// ���� �̺�Ʈ ����
		CurEventIndex++;
	}
}
