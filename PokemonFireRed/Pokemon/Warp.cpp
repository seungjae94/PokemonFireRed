#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include "EventCondition.h"

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

void AWarp::RegisterEvents()
{
	AEventTrigger::RegisterEvents();

	UEventCondition Cond1 = UEventCondition(EEventTriggerAction::Notice);
	UEventManager::Register(this, Cond1, [this]() {return Event0();});
	UEventManager::Register(this, Cond1, [this]() {return Event1();});
	UEventManager::Register(this, Cond1, [this]() {return Event2();});
	UEventManager::Register(this, Cond1, [this]() {return Event3();});
	UEventManager::Register(this, Cond1, [this]() {return Event4();});
}

bool AWarp::Event0()
{
	return UEventManager::StealPlayerControl();
}

bool AWarp::Event1()
{
	return UEventManager::MoveActor(GetWorld()->GetName(), "Player", { MoveDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event2()
{
	return UEventManager::ChangeMap(TargetMapName, TargetPoint);
}

bool AWarp::Event3()
{
	return UEventManager::ChangeDirection(TargetMapName, "Player", { MoveDirection.ToFVector() });
}

bool AWarp::Event4()
{
	return UEventManager::GiveBackPlayerControl();
}
