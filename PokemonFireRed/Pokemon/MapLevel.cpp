#include "MapLevel.h"
#include <string>
#include <vector>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include "Warp.h"
#include "PokemonDebug.h"
#include "EventManager.h"


UMapLevel::UMapLevel() 
{
}

UMapLevel::~UMapLevel() 
{
}

void UMapLevel::BeginPlay()
{
	static bool IsCharacterResourceLoaded = false;

	UPokemonLevel::BeginPlay();

	// �⺻ ��� ����
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	// �÷��̾�, Npc ���ҽ� �ε� (�� ���ӿ� ���� 1���� ����)
	if (false == IsCharacterResourceLoaded)
	{
		LoadCharacterResources();
	}
	
	// �� ���ҽ� �ε�
	std::string MapName = GetName();
	CurDir.Move(MapName);

	std::list<UEngineFile> Files = CurDir.AllFile();

	for (UEngineFile& File : Files)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// ���� ����
	Player = SpawnPlayer({0, 0});
	Map = SpawnActor<AMap>();

	// �÷��̾ ���� ������ ����
	Player->SetMap(Map);

	// �� ����
	FVector MapInitialPos = { -Global::F_TILE_SIZE * 0.5f, -Global::F_TILE_SIZE * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);
	Map->SetName(MapName + "Map");
	Map->SetBackgroundImage(MapName + "Background.png");
	Map->SetForegroundImage(MapName + "Foreground.png");
	Map->SetCollisionImage(MapName + "Collision.png");
	Map->SetCollisionRendererActive(false);

	IsCharacterResourceLoaded = true;
}

void UMapLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	// ����� ���
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

	if (UEngineInput::IsDown('F'))
	{
		PokemonDebug::ReportFrameRate(_DeltaTime);
	}
}

void UMapLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
}

void UMapLevel::LoadCharacterResources()
{
	CurDir.Move("Character");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// Idle �ִϸ��̼��� �ε��� ĳ���� �̸��� ����
	std::vector<std::string> IdleCharacterNames = {
		"Player",
		"FatMan",
		"PlayersMom"
	};

	// Idle �ִϸ��̼� ���ҽ� �ε�
	for (std::string& Name : IdleCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "Idle.png", 4, 1);
	}

	// Walk �ִϸ��̼��� �ε��� ĳ���� �̸��� ����
	std::vector<std::string> WalkCharacterNames = {
		"Player",
		"FatMan"
	};

	// Walk �ִϸ��̼� ���ҽ� �ε�
	for (std::string& Name : WalkCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkDown.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkUp.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkLeft.png", 4, 1);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkRight.png", 4, 1);
	}
	
	// �÷��̾� ���� �ִϸ��̼� ���ҽ� �ε�
	UEngineResourcesManager::GetInst().CuttingImage("PlayerJumpDown.png", 53, 1);

	CurDir.MoveParent();
}

