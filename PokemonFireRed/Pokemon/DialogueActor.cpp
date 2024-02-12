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
	UEventManager::Register(this, Cond1, [this]() {return Event0();});
	UEventManager::Register(this, Cond1, [this]() {return Event1();});
	UEventManager::Register(this, Cond1, [this]() {return Event2();});
}

bool ADialogueActor::Event0()
{
	return UEventManager::StealPlayerControl();
}

bool ADialogueActor::Event1()
{
	return UEventManager::Chat({L"MOM: ...Right.", L"All boys leave home someday."}, EFontColor::White, true);
}

bool ADialogueActor::Event2()
{
	return UEventManager::GiveBackPlayerControl();
}

