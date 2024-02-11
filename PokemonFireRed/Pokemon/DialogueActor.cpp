#include "DialogueActor.h"
#include "EventManager.h"
#include "EventCondition.h"

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

	UEventCondition Cond1 = UEventCondition(EEventTriggerAction::Click);
	UEventManager::Register(this, Cond1, [this]() {return Event0();});
	UEventManager::Register(this, Cond1, [this]() {return Event1();});
}

bool ADialogueActor::Event0()
{
	return UEventManager::StealPlayerControl();
}

bool ADialogueActor::Event1()
{
	EngineDebug::OutPutDebugText("대화 이벤트");
	return UEventManager::GiveBackPlayerControl();
}

