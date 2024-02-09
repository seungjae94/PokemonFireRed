#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include "PokemonMath.h"
#include <EngineCore/EngineCore.h>

std::string AWarp::CurTargetMapName;
FTileVector AWarp::CurTargetPoint = { 70, 142 };			// 주인공 집 앞
FTileVector AWarp::CurTargetDirection = { 0, 1 };  // 아래 방향

AWarp::AWarp() 
	: AEventActor()
{
}

AWarp::~AWarp() 
{
}

void AWarp::TriggerEvent()
{
	CurTargetMapName = TargetMapName;
	CurTargetPoint = TargetPoint;
	CurTargetDirection = Player->GetDirection();
	EventProcessor->Work();
}

void AWarp::BeginPlay()
{
	AEventActor::BeginPlay();

	EventProcessor->Register([this](float _DeltaTime) {return Event1(_DeltaTime);});
	EventProcessor->Register([this](float _DeltaTime) {return Event2(_DeltaTime);});
	EventProcessor->Register([this](float _DeltaTime) {return Event3(_DeltaTime);});
	EventProcessor->Register([this](float _DeltaTime) {return Event4(_DeltaTime);});
}

bool AWarp::Event1(float _DeltaTime)
{
	EngineDebug::OutPutDebugText("E1");
	return EventDelegate.MoveActor(_DeltaTime, Player, { CurTargetDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event2(float _DeltaTime)
{
	GEngine->ChangeLevel(TargetMapName);
	return true;
}

bool AWarp::Event3(float _DeltaTime)
{
	EngineDebug::OutPutDebugText("E3");
	return EventDelegate.MoveActor(_DeltaTime, Player, { CurTargetDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event4(float _DeltaTime)
{
	Player->StateChange(EPlayerState::Idle);
	return true;
}
