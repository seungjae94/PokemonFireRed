#include "DialogueActor.h"
#include "EventManager.h"

ADialogueActor::ADialogueActor()
{
}

ADialogueActor::~ADialogueActor()
{
}

void ADialogueActor::BeginPlay()
{
	AEventTrigger::BeginPlay();
}

void ADialogueActor::RegisterEvents()
{
	AEventTrigger::RegisterEvents();

	UEventManager::Register(this, [this]() {return Event1();});
}

bool ADialogueActor::Event1()
{
	EngineDebug::OutPutDebugText("대화 이벤트");
	return UEventManager::Finish(GetWorld()->GetName());
}

