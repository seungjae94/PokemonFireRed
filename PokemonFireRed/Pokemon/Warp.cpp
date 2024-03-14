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

void AWarp::SetTargetBgm(std::string_view _TargetBgm)
{
	TargetBgm = _TargetBgm;
}

bool AWarp::CheckPlayerDirection()
{
	FTileVector CurPlayerDirection = UEventManager::FindCurLevelTarget<APlayer>(Global::Player)->GetDirection();
	return CurPlayerDirection == TargetDirection;
}
