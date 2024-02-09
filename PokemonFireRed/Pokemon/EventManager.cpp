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
			MsgBoxAssert("이벤트 프로세서가 이미 삭제되어 있습니다.");
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
		MsgBoxAssert("이미 이벤트 프로세서를 할당한 이벤트 액터에 다시 이벤트 프로세서를 할당하려고 했습니다.");
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
