#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

void AWarp::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition WarpCond = UEventCondition(EEventTriggerAction::Notice);
	WarpCond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, WarpCond,
		ES::Start(true)
		// >> ES::FadeOutAsync(0.5f)
		>> ES::MoveActor(GetWorld()->GetName(), "Player", { MoveDirection.ToFVector() }, 1.8f)
		>> ES::FadeOut(0.5f) // delete
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, "Player", TargetPoint)
		>> ES::ChangeDirection(TargetMapName, "Player", MoveDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);
}

bool AWarp::CheckPlayerDirection()
{
	FTileVector CurPlayerDirection = UEventManager::GetCurPlayer()->GetDirection();
	return CurPlayerDirection == MoveDirection;
}
