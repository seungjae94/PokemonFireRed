#include "MapLevel.h"
#include <string>
#include <vector>
#include <EngineCore/EngineCore.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include "Warp.h"
#include "PokemonDebug.h"
#include "EventManager.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"
#include "Map.h"
#include "MenuWindow.h"
#include "DialogueWindow.h"
#include "BlackScreen.h"


UMapLevel::UMapLevel()
{
}

UMapLevel::~UMapLevel()
{
}

void UMapLevel::BeginPlay()
{
	static bool IsCommonResourceLoaded = false;

	UPokemonLevel::BeginPlay();

	// �⺻ ��� ����
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	// �÷��̾�, Npc, UI ���ҽ� �ε� (�� ���ӿ� ���� 1���� ����)
	if (false == IsCommonResourceLoaded)
	{
		LoadCharacterResources();
		IsCommonResourceLoaded = true;
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
	Player = SpawnPlayer({ 0, 0 });
	Map = SpawnActor<AMap>();

	// �÷��̾� ������ ���ε�
	Player->SetMap(Map);

	// �� ������ ���ε�
	FVector MapInitialPos = { -Global::F_TILE_SIZE * 0.5f, -Global::F_TILE_SIZE * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);
	Map->SetName(MapName + "Map");
	Map->SetBackgroundImage(MapName + "Background.png");
	Map->SetForegroundImage(MapName + "Foreground.png");
	Map->SetCollisionImage(MapName + "Collision.png");
	Map->SetCollisionRendererActive(false);

	// �޴�â ����
	MenuWindow = SpawnUIElement<AMenuWindow>("MenuWindow");

	// ��ȭâ ����
	DialogueWindow = SpawnUIElement<ADialogueWindow>("DialogueWindow");

	// ����â ����
	ABlackScreen* BlackScreen = SpawnUIElement<ABlackScreen>("BlackScreen");

	// UI Off
	MenuWindow->ActiveOff();
	MenuWindow->AllRenderersActiveOff();
	DialogueWindow->ActiveOff();
	DialogueWindow->AllRenderersActiveOff();

	// ���̵� �� �̺�Ʈ�� Ʈ���� ����
	UEventTargetInitialSetting Setting = UEventTargetInitialSetting(GetName() + "MapFadeInTrigger");
	FadeInTrigger = SpawnEventTrigger<AEventTrigger>(Setting);

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Direct);
	UEventManager::RegisterEvent(FadeInTrigger, Cond, ES::Start(false) >> ES::FadeIn(0.75f) >> ES::End(false));
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

	UEventManager::TriggerEvent(FadeInTrigger);
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
		UEngineResourcesManager::GetInst().CuttingImage(Name + "Idle.png", 4, 2);
	}

	// Walk �ִϸ��̼��� �ε��� ĳ���� �̸��� ����
	std::vector<std::string> WalkCharacterNames = {
		"Player",
		"FatMan"
	};

	// Walk �ִϸ��̼� ���ҽ� �ε�
	for (std::string& Name : WalkCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkDown.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkUp.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkLeft.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkRight.png", 4, 2);
	}

	// �÷��̾� ���� �ִϸ��̼� ���ҽ� �ε�
	// - ��ü 53�� ������ + ��ü 53�� ������
	UEngineResourcesManager::GetInst().CuttingImage("PlayerJumpDown.png", 53, 2);

	CurDir.MoveParent();
}