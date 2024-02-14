#include "DialogueActor.h"
#include "EventManager.h"
#include "EventCondition.h"
#include "PokemonText.h"

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
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event0();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event1();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event2();});
}

bool ADialogueActor::Event0()
{
	return UEventManager::StealPlayerControl();
}

bool ADialogueActor::Event1()
{
	return UEventManager::Chat( Dialogue, TextColor, 16, true);
}

bool ADialogueActor::Event2()
{
	return UEventManager::GiveBackPlayerControl();
}

