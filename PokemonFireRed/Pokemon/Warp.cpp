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
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event5));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event6));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event7));
	UEventManager::RegisterEvent(this, Cond1, ToEvent(Event8));
}

bool AWarp::Event0()
{
	return UEventManager::StealPlayerControl();
}

bool AWarp::Event1()
{
	static bool MoveEnd = false;
	static bool FadeOutEnd = false;

	if (false == MoveEnd)
	{
		MoveEnd = UEventManager::MoveActor(GetWorld()->GetName(), "Player", { MoveDirection.ToFVector() }, 1.8f);
	}

	if (false == FadeOutEnd)
	{
		FadeOutEnd = UEventManager::FadeOut(0.5f);
	}

	if (true == MoveEnd && true == FadeOutEnd)
	{
		MoveEnd = false;
		FadeOutEnd = false;
		return true;
	}
}

bool AWarp::Event2()
{
	return true;
}

bool AWarp::Event3()
{
	return UEventManager::ChangeMap(TargetMapName, TargetPoint);
}

bool AWarp::Event4()
{
	return UEventManager::ChangeDirection(TargetMapName, "Player", { MoveDirection.ToFVector() });
}

bool AWarp::Event5()
{
	return UEventManager::FadeOut(0.0f);
}

bool AWarp::Event6()
{
	return UEventManager::Wait(0.5f);
}

bool AWarp::Event7()
{
	return UEventManager::FadeIn(0.5f);
}

bool AWarp::Event8()
{
	return UEventManager::GiveBackPlayerControl();
}

bool AWarp::CheckPlayerDirection()
{
	FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
	return CurPlayerDirection == MoveDirection;
}
