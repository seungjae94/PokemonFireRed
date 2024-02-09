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
	IsTriggered = true;
}

void AWarp::BeginPlay()
{
	AActor::BeginPlay();
	RegisterEvent([this](AEventActor* _Actor, float _DeltaTime) {return Event1(_DeltaTime);});
	RegisterEvent([this](AEventActor* _Actor, float _DeltaTime) {return Event2(_DeltaTime);});
}

bool AWarp::Event1(float _DeltaTime)
{
	return EventDelegate.MoveActor(_DeltaTime, Player, { CurTargetDirection.ToFVector() }, 1.8f);
}

bool AWarp::Event2(float _DeltaTime)
{
	Player->StateChange(EPlayerState::Idle);
	GEngine->ChangeLevel(TargetMapName);
	return true;
}
