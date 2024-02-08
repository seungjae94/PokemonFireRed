#include "MapLevel.h"
#include <string>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"

UMapLevel::UMapLevel() 
{
}

UMapLevel::~UMapLevel() 
{
}

void UMapLevel::BeginPlay()
{
	static bool IsCalled = false;

	if (true == IsCalled)
	{
		return;
	}
	IsCalled = true;

	// �÷��̾� ���ҽ� �ε�
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({".png", ".bmp"}, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// �÷��̾� �ִϸ��̼� ���ҽ� �ε�
	UEngineResourcesManager::GetInst().CuttingImage("WalkDown.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkUp.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkLeft.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("WalkRight.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("JumpDown.png", 53, 1);

	// ���� ����
	Player = SpawnActor<APlayer>();
	Map = SpawnActor<AMap>();

	Player->SetMap(Map);
	Map->SetPlayer(Player);
}

void UMapLevel::Tick(float _DeltaTime)
{
	if (EngineInput::IsDown(VK_F1))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}

	if (EngineInput::IsDown(VK_F2))
	{
		PokemonDebug::ReportPosition(Player, "Player");
	}

	if (EngineInput::IsDown(VK_F3))
	{
		PokemonDebug::ReportPosition(Map, "Map");
	}
}

