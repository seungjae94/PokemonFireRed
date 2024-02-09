#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include <EngineCore/EngineCore.h>

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

void AWarp::RegisterEvents()
{
	UEventManager::Register(this, [this]() {return Event1();});
	UEventManager::Register(this, [this]() {return Event2();});
	UEventManager::Register(this, [this]() {return Event3();});
	UEventManager::Register(this, [this]() {return Event4();});
}

bool AWarp::Event1()
{
	return UEventManager::MoveActor(GetWorld()->GetName(), "Player", { MoveDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event2()
{
	return UEventManager::ChangeMap(GetWorld()->GetName(), TargetMapName, TargetPoint);
}

bool AWarp::Event3()
{
	return UEventManager::MoveActor(TargetMapName, "Player", { MoveDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event4()
{
	return UEventManager::Finish(TargetMapName);
}
