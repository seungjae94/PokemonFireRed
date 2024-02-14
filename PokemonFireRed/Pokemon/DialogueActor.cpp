#include "DialogueActor.h"
#include "EventManager.h"
#include "EventCondition.h"
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

void ADialogueActor::RegisterEvents()
{
	AEventTrigger::RegisterEvents();

	UEventCondition Cond1 = UEventCondition(EEventTriggerAction::Click);
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event0();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event1();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event2();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event3();});
	UEventManager::RegisterEvent(this, Cond1, [this]() {return Event4();});
}

bool ADialogueActor::Event0()
{
	BeforeChatDirection = Direction;
	return UEventManager::StealPlayerControl();
}

bool ADialogueActor::Event1()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	return UEventManager::ChangeDirection(GetWorld()->GetName(), GetName(), -CurPlayer->GetDirection());
}

bool ADialogueActor::Event2()
{
	return UEventManager::Chat( Dialogue, TextColor, 16, true);
}

bool ADialogueActor::Event3()
{
	return UEventManager::ChangeDirection(GetWorld()->GetName(), GetName(), BeforeChatDirection);
}

bool ADialogueActor::Event4()
{
	return UEventManager::GiveBackPlayerControl();
}
