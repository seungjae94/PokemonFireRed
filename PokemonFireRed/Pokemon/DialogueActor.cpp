#include "DialogueActor.h"
#include "EventManager.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "PokemonText.h"
#include "Player.h"

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

void ADialogueActor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond1 = UEventCondition(EEventTriggerAction::Click);
	UEventManager::RegisterEvent(this, Cond1,
		ES::Start(true) 
		// >> ES::LookPlayer()
		>> ES::Chat(Dialogue, TextColor, 16, true) 
		>> ES::End(true)
	);
}