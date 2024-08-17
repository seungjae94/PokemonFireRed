#include "MapLevel.h"
#include <string>
#include <vector>
#include <EngineCore/EngineCore.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineSound.h>
#include "PokemonDebug.h"
#include "EventManager.h"
#include "EventStream.h"
#include "PlayerCharacter.h"
#include "Map.h"
#include "Surprise.h"
#include "MenuCanvas.h"
#include "DialogueWindow.h"
#include "MapNameCanvas.h"
#include "AnimatedFlower.h"
#include "AnimatedSea.h"
#include "WildBattleTrigger.h"
#include "EventMacros.h"

UMapLevel::UMapLevel()
{
}

UMapLevel::~UMapLevel()
{
}

std::string UMapLevel::GetAreaName() const
{
	return AreaName;
}

std::string UMapLevel::GetAreaBgm() const
{
	return AreaBgm;
}

void UMapLevel::SetAreaName(std::string_view _AreaName)
{
	AreaName = _AreaName;
}

void UMapLevel::SetAreaBgm(std::string_view _AreaBgm)
{
	AreaBgm = _AreaBgm;
}

void UMapLevel::BeginPlay()
{
	static bool IsCommonResourceLoaded = false;

	UPokemonLevel::BeginPlay();

	// 기본 경로 설정
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("MapLevel");

	// 캐릭터, 트리거, 타일 리소스 로드 (전 게임에 걸쳐 1번만 실행)
	if (false == IsCommonResourceLoaded)
	{
		LoadCharacterResources();
		LoadObjectResources();
		LoadTileResources();
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

	UEventTargetSetting SurpriseSetting;
	SurpriseSetting.SetName(EN::Surprise);
	SurpriseSetting.SetDirection(FTileVector::Down);
	SurpriseSetting.SetPoint({10000, 10000});
	SurpriseSetting.SetCollidable(false);
	ASurprise* Surprise = SpawnEventTarget<ASurprise>(SurpriseSetting);

	// 플레이어 데이터 바인딩
	Player->SetMap(Map);

	// 맵 데이터 바인딩
	FVector MapInitialPos = { -Global::FloatTileSize * 0.5f, -Global::FloatTileSize * 0.5f };
	Map->SetActorLocation(MapInitialPos);
	Map->SetPlayer(Player);
	Map->SetName(MapName + "Map");
	Map->SetBackgroundImage(MapName + "Background.png");
	Map->SetForegroundImage(MapName + "Foreground.png");
	Map->SetCollisionImage(MapName + "Collision.png");
	Map->SetCollisionRendererActive(false);

	// 메뉴창 생성
	AMenuCanvas* MenuCanvas = SpawnCommonCanvas<AMenuCanvas>(Global::MenuWindow);
	MenuCanvas->SetActive(false);

	// 맵 이름 표시창 생성
	AMapNameCanvas* MapNameCanvas = SpawnCommonCanvas<AMapNameCanvas>(Global::MapNameWindow);
	MapNameCanvas->SetActive(false);

	// 야생 배틀 트리거 생성
	SpawnEventTrigger<AWildBattleTrigger>(Global::WildBattleTrigger);

	// 페이드 인 이벤트용 트리거 생성
	UEventTargetSetting Setting; 
	Setting.SetName("FadeInTrigger");
	FadeInTrigger = SpawnEventTrigger<AEventTrigger>(Setting);

	FadeInTrigger->RegisterEvent(
		EEventTriggerAction::Direct,
		SKIP_CHECK,
		ES::Start(false) >> ES::FadeIn(0.75f) >> ES::End(false)
	);
}

void UMapLevel::Tick(float _DeltaTime)
{
	UPokemonLevel::Tick(_DeltaTime);

	// 플레이어 위치 확인
	if (UEngineInput::IsDown('P'))
	{
		PokemonDebug::ReportPosition(Player, Global::PlayerCharacter);
	}

	// 콜리전 맵 확인
	if (UEngineInput::IsDown('O'))
	{
		bool ActiveValue = Map->IsCollisionRendererActive();
		Map->SetCollisionRendererActive(!ActiveValue);
	}
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

	// Idle 애니메이션을 로드할 캐릭터 리소스 이름을 정의
	std::vector<std::string> IdleCharacterNames = {
		EN::PlayerCharacter,
		EN::TechMan,
		EN::Mom,
		EN::RivalGreen,
		EN::Oak,
		EN::Nurse,
		EN::ShopOwner,
		EN::BugCatcher,
		EN::Camper,
		EN::LeaderBrock
	};

	// Idle 애니메이션 리소스 로드
	for (std::string& Name : IdleCharacterNames)
	{
		UEngineResourcesManager::GetInst().CuttingImage(Name + "Idle.png", 4, 2);
	}

	// Walk 애니메이션을 로드할 캐릭터 이름을 정의
	std::vector<std::string> WalkCharacterNames = {
		EN::PlayerCharacter,
		EN::TechMan,
		EN::RivalGreen,
		EN::Oak,
		EN::BugCatcher,
		EN::Camper
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
	UEngineResourcesManager::GetInst().CuttingImage("PlayerCharacterJumpDown.png", 53, 2);

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

	// 로드할 폴더 이름 정의
	std::vector<std::string> LoadFolderNames = {
		RN::GreenDoor,
		RN::RedDoor,
		RN::SlideDoor,
		RN::GymDoor
	};

	// 폴더 로드
	for (std::string& Name : LoadFolderNames)
	{
		UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath(Name));
	}

	// BallHeal, Surprise 로드
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

	// 이미지 분할
	UEngineResourcesManager::GetInst().CuttingImage("AnimatedFlower.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("AnimatedSea.png", 8, 1);

	CurDir.MoveParent();
}
