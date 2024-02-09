#include "MapLevel.h"
#include <string>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include "Warp.h"
#include "PokemonDebug.h"


UMapLevel::UMapLevel() 
{
}

UMapLevel::~UMapLevel() 
{
}

void UMapLevel::BeginPlay()
{
	static bool IsPlayerResourceLoaded = false;

	// 기본 경로 설정
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	// 플레이어 리소스 로드 (전 게임에 걸쳐 1번만 실행)
	if (false == IsPlayerResourceLoaded)
	{
		std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, false);
		for (UEngineFile& File : AllFiles)
		{
			std::string Path = File.GetFullPath();
			UEngineResourcesManager::GetInst().LoadImg(Path);
		}

		// 플레이어 애니메이션 리소스 로드
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkDown.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkUp.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkLeft.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkRight.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerJumpDown.png", 53, 1);
	}
	
	// 액터 생성
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	// 맵의 좌표 설정
	FVector MapInitialPos = { -Global::F_TILE_SIZE * 0.5f, -Global::F_TILE_SIZE * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);

	// 액터의 멤버 설정
	Player->SetMap(Map);
	Player->SetMapLevel(this);

	IsPlayerResourceLoaded = true;
}



void UMapLevel::Tick(float _DeltaTime)
{
	if (UEngineInput::IsDown(VK_F1))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}

	if (UEngineInput::IsDown(VK_F2))
	{
		PokemonDebug::ReportPosition(Player, "Player");
	}

	if (UEngineInput::IsDown(VK_F3))
	{
		PokemonDebug::ReportPosition(Map, "Map");
	}

	if (UEngineInput::IsDown('O'))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}
}

void UMapLevel::LevelStart(ULevel* _PrevLevel)
{
	ULevel::LevelStart(_PrevLevel);

	// 플레이어 위치 지정
	const FTileVector& CurTargetPoint = AWarp::GetCurTargetPoint();
	Player->SetTilePoint(CurTargetPoint);

	const FTileVector& CurTargetDirection = AWarp::GetCurTargetDirection();
	Player->SetDirection(CurTargetDirection);

	Player->StateChange(EPlayerState::Idle, true);
}

