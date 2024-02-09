#include "EventManager.h"
#include "EventActor.h"
#include "EventManager.h"

std::map<AEventActor*, UEventProcessor*> UEventManager::AllProcessors;

UEventManager::UEventManager()
{
}

UEventManager::~UEventManager()
{
	for (std::pair<AEventActor* const, UEventProcessor*>& Pair : AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (nullptr == Processor)
		{
			MsgBoxAssert("�̺�Ʈ ���μ����� �̹� �����Ǿ� �ֽ��ϴ�.");
			return;
		}

		delete Processor;
		Pair.second = nullptr;
	}

	AllProcessors.clear();
}

UEventProcessor* UEventManager::CreateEventProcessor(AEventActor* _Caller)
{
	if (AllProcessors.contains(_Caller))
	{
		MsgBoxAssert("�̹� �̺�Ʈ ���μ����� �Ҵ��� �̺�Ʈ ���Ϳ� �ٽ� �̺�Ʈ ���μ����� �Ҵ��Ϸ��� �߽��ϴ�.");
		return nullptr;
	}

	UEventProcessor* NewProcessor = new UEventProcessor();
	AllProcessors[_Caller] = NewProcessor;
	return NewProcessor;
}


void UEventManager::Tick(float _DeltaTime)
{
	for (std::pair<AEventActor* const, UEventProcessor*>& Pair: AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (Processor->IsWorking())
		{
			Processor->Tick(_DeltaTime);
		}
	}
}
