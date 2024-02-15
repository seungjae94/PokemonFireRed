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

	// 기본 경로 설정
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("MapLevel");

	// 플레이어, Npc, UI 리소스 로드 (전 게임에 걸쳐 1번만 실행)
	if (false == IsCommonResourceLoaded)
	{
		LoadCharacterResources();
		IsCommonResourceLoaded = true;
	}

	// 맵 리소스 로드
	std::string MapName = GetName();
	CurDir.Move(MapName);

	std::list<UEngineFile> Files = CurDir.AllFile();

	for (UEngineFile& File : Files)
	{
		std::string Path = File.GetFullPath();
		UEngineResourcesManager::GetInst().LoadImg(Path);
	}

	// 액터 생성
	Player = SpawnPlayer({ 0, 0 });
	Map = SpawnActor<AMap>();

	// 플레이어 데이터 바인딩
	Player->SetMap(Map);

	// 맵 데이터 바인딩
	FVector MapInitialPos = { -Global::F_TILE_SIZE * 0.5f, -Global::F_TILE_SIZE * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);
	Map->SetName(MapName + "Map");
	Map->SetBackgroundImage(MapName + "Background.png");
	Map->SetForegroundImage(MapName + "Foreground.png");
	Map->SetCollisionImage(MapName + "Collision.png");
	Map->SetCollisionRendererActive(false);

	// 메뉴창 생성
	MenuWindow = SpawnUIElement<AMenuWindow>("MenuWindow");

	// 대화창 생성
	DialogueWindow = SpawnUIElement<ADialogueWindow>("DialogueWindow");

	// 검은창 생성
	ABlackScreen* BlackScreen = SpawnUIElement<ABlackScreen>("BlackScreen");

	// UI Off
	MenuWindow->ActiveOff();
	MenuWindow->AllRenderersActiveOff();
	DialogueWindow->ActiveOff();
	DialogueWindow->AllRenderersActiveOff();

	// 페이드 인 이벤트용 트리거 생성
	UEventTargetInitialSetting Setting = UEventTargetInitialSetting(GetName() + "MapFadeInTrigger");
	FadeInTrigger = SpawnEventTrigger<AEventTrigger>(Setting);

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Direct);
	UEventManager::RegisterEvent(FadeInTrigger, Cond, ES::Start(false) >> ES::FadeIn(0.75f) >> ES::End(false));
}

void UMapLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	// 디버깅 기능
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

	// Idle 애니메이션을 로드할 캐릭터 이름을 정의
	std::vector<std::string> IdleCharacterNames = {
		"Player",
		"FatMan",
		"PlayersMom"
	};

	// Idle 애니메이션 리소스 로드
	for (std::string& Name : IdleCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "Idle.png", 4, 2);
	}

	// Walk 애니메이션을 로드할 캐릭터 이름을 정의
	std::vector<std::string> WalkCharacterNames = {
		"Player",
		"FatMan"
	};

	// Walk 애니메이션 리소스 로드
	for (std::string& Name : WalkCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkDown.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkUp.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkLeft.png", 4, 2);
		UEngineResourcesManager::GetInst().CuttingImage(Name + "WalkRight.png", 4, 2);
	}

	// 플레이어 점프 애니메이션 리소스 로드
	// - 상체 53개 프레임 + 하체 53개 프레임
	UEngineResourcesManager::GetInst().CuttingImage("PlayerJumpDown.png", 53, 2);

	CurDir.MoveParent();
}