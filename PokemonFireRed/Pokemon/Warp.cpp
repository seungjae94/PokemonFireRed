#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include "EventCondition.h"
#include "Player.h"

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
	Cond1.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event0));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event1));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event2));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event3));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event4));
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

bool AWarp::CheckPlayerDirection()
{
	FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
	return CurPlayerDirection == MoveDirection;
}
