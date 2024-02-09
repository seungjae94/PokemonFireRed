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

	// �⺻ ��� ����
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	// �÷��̾� ���ҽ� �ε� (�� ���ӿ� ���� 1���� ����)
	if (false == IsPlayerResourceLoaded)
	{
		std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, false);
		for (UEngineFile& File : AllFiles)
		{
			std::string Path = File.GetFullPath();
			UEngineResourcesManager::GetInst().LoadImg(Path);
		}

		// �÷��̾� �ִϸ��̼� ���ҽ� �ε�
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkDown.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkUp.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkLeft.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerWalkRight.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage("PlayerJumpDown.png", 53, 1);
	}
	
	// ���� ����
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	// ���� ��ǥ ����
	FVector MapInitialPos = { -Global::F_TILE_SIZE * 0.5f, -Global::F_TILE_SIZE * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);

	// ������ ��� ����
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

	// �÷��̾� ��ġ ����
	const FTileVector& CurTargetPoint = AWarp::GetCurTargetPoint();
	Player->SetTilePoint(CurTargetPoint);

	const FTileVector& CurTargetDirection = AWarp::GetCurTargetDirection();
	Player->SetDirection(CurTargetDirection);

	Player->StateChange(EPlayerState::Idle, true);
}

