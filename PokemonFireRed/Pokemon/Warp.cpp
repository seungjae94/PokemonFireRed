#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include <EngineCore/EngineCore.h>

std::string AWarp::CurTargetMapName;
FTileVector AWarp::CurTargetPoint = { 70, 142 };			// 주인공 집 앞
FTileVector AWarp::CurTargetDirection = FTileVector::Down;  // 아래 방향

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

void AWarp::TriggerEvent()
{
	EngineDebug::OutPutDebugText("장소를 이동합니다.");
	GEngine->ChangeLevel(TargetMapName);
	CurTargetMapName = TargetMapName;
	CurTargetPoint = TargetPoint;
	CurTargetDirection = Player->GetDirection();
	GetWorld()->SetCameraPos(Player->GetActorLocation() - Global::HALF_SCREEN);
}

void AWarp::BeginPlay()
{
	AActor::BeginPlay();
}

void AWarp::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (UEngineInput::IsDown(VK_F4))
	{
		PokemonDebug::ReportPosition(this, "Warp");
	}
}

