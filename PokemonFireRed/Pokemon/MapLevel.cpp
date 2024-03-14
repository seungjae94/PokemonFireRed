#include "MapLevel.h"
#include <string>
#include <vector>
#include <EngineCore/EngineCore.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineSound.h>
#include "PokemonDebug.h"
#include "EventManager.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"
#include "Map.h"
#include "Surprise.h"
#include "MenuCanvas.h"
#include "DialogueWindow.h"
#include "MapNameCanvas.h"
#include "AnimatedFlower.h"
#include "AnimatedSea.h"
#include "WildBattleTrigger.h"

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
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("MapLevel");

	// ĳ����, Ʈ����, Ÿ�� ���ҽ� �ε� (�� ���ӿ� ���� 1���� ����)
	if (false == IsCommonResourceLoaded)
	{
		LoadCharacterResources();
		LoadObjectResources();
		LoadTileResources();
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

	UEventTargetSetting SurpriseSetting;
	SurpriseSetting.SetName(EN::Surprise);
	SurpriseSetting.SetDirection(FTileVector::Down);
	SurpriseSetting.SetPoint({10000, 10000});
	SurpriseSetting.SetCollidable(false);
	ASurprise* Surprise = SpawnEventTarget<ASurprise>(SurpriseSetting);

	// �÷��̾� ������ ���ε�
	Player->SetMap(Map);

	// �� ������ ���ε�
	FVector MapInitialPos = { -Global::FloatTileSize * 0.5f, -Global::FloatTileSize * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);
	Map->SetName(MapName + "Map");
	Map->SetBackgroundImage(MapName + "Background.png");
	Map->SetForegroundImage(MapName + "Foreground.png");
	Map->SetCollisionImage(MapName + "Collision.png");
	Map->SetCollisionRendererActive(false);

	// �޴�â ����
	AMenuCanvas* MenuCanvas = SpawnCommonCanvas<AMenuCanvas>(Global::MenuWindow);
	MenuCanvas->SetActive(false);

	// �� �̸� ǥ��â ����
	AMapNameCanvas* MapNameCanvas = SpawnCommonCanvas<AMapNameCanvas>(Global::MapNameWindow);
	MapNameCanvas->SetActive(false);

	// �߻� ��Ʋ Ʈ���� ����
	SpawnEventTrigger<AWildBattleTrigger>(Global::WildBattleTrigger);

	// ���̵� �� �̺�Ʈ�� Ʈ���� ����
	UEventTargetSetting Setting; 
	Setting.SetName("FadeInTrigger");
	FadeInTrigger = SpawnEventTrigger<AEventTrigger>(Setting);

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Direct);
	UEventManager::RegisterEvent(FadeInTrigger, Cond, ES::Start(false) >> ES::FadeIn(0.75f) >> ES::End(false));
}

void UMapLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	// ����� ���
	if (UEngineInput::IsDown('P'))
	{
		PokemonDebug::ReportPosition(Player, Global::Player);
	}

	if (UEngineInput::IsDown('O'))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}

	PokemonDebug::ReportFrameRate(_DeltaTime, 100.0f);
}

void UMapLevel::LevelStart(ULevel* _PrevLevel)
{
	UPokemonLevel::LevelStart(_PrevLevel);
	SetCameraPos(Player->GetActorLocation() - Global::HalfScreen);
}

void UMapLevel::DrawFlowers(const std::vector<FTileVector>& _Points)
{
	for (const FTileVector& Point : _Points)
	{
		AnimatedFlower* Flower = SpawnActor<AnimatedFlower>();
		Flower->SetActorLocation(Point.ToFVector());
	}
}

void UMapLevel::DrawSeas(const std::vector<FTileVector>& _Points)
{
	for (const FTileVector& Point : _Points)
	{
		AnimatedSea* Sea = SpawnActor<AnimatedSea>();
		Sea->SetActorLocation(Point.ToFVector());
	}
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

	// Idle �ִϸ��̼��� �ε��� ĳ���� ���ҽ� �̸��� ����
	std::vector<std::string> IdleCharacterNames = {
		EN::Player,
		EN::TechMan,
		EN::PlayersMom,
		EN::RivalGreen,
		EN::Oak,
		EN::Nurse,
		EN::ShopOwner,
		EN::BugCatcher,
		EN::Camper,
		EN::LeaderBrock
	};

	// Idle �ִϸ��̼� ���ҽ� �ε�
	for (std::string& Name : IdleCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "Idle.png", 4, 2);
	}

	// Walk �ִϸ��̼��� �ε��� ĳ���� �̸��� ����
	std::vector<std::string> WalkCharacterNames = {
		EN::Player,
		EN::TechMan,
		EN::RivalGreen,
		EN::Oak,
		EN::BugCatcher,
		EN::Camper
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

void UMapLevel::LoadObjectResources()
{
	CurDir.Move("Object");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// �ε��� ���� �̸� ����
	std::vector<std::string> LoadFolderNames = {
		RN::GreenDoor,
		RN::RedDoor,
		RN::SlideDoor,
		RN::GymDoor
	};

	// ���� �ε�
	for (std::string& Name : LoadFolderNames)
	{
		UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath(Name));
	}

	// BallHeal, Surprise �ε�
	UEngineResourcesManager::GetInst().CuttingImage(RN::BallHeal, 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage(RN::Surprise, 8, 1);

	CurDir.MoveParent();
}

void UMapLevel::LoadTileResources()
{
	CurDir.Move("Tile");

	std::list<UEngineFile> AllFiles = CurDir.AllFile({ ".png", ".bmp" }, false);
	for (UEngineFile& File : AllFiles)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// �̹��� ����
	UEngineResourcesManager::GetInst().CuttingImage("AnimatedFlower.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("AnimatedSea.png", 8, 1);

	CurDir.MoveParent();
}
