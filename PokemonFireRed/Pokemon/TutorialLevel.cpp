#include "TutorialLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "TutorialLevelManager.h"

UTutorialLevel::UTutorialLevel()
{
}

UTutorialLevel::~UTutorialLevel()
{
}

void UTutorialLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("TutorialLevel");

	// 리소스 로드
	std::list<UEngineFile> Files = CurDir.AllFile({".png"}, false);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	// 액터 생성
	Manager = SpawnActor<ATutorialLevelManager>();

	// 중간 전환자
	UEventCondition DefaultCondition;

	AEventTrigger* Fader = SpawnEventTrigger<AEventTrigger>(Global::TutorialLevelFader);
	UEventManager::RegisterEvent(Fader, DefaultCondition,
		ES::Start(false)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(false)
	);

	// 튜토리얼 레벨 -> 맵 레벨 전환자
	UEventTargetSetting LevelChangerInit; 
	LevelChangerInit.SetName("MapLevelChanger");
	MapLevelChanger = SpawnEventTrigger<AEventTrigger>(LevelChangerInit);
	UEventManager::RegisterEvent(MapLevelChanger, DefaultCondition,
		ES::Start(false)
		>> ES::FadeOut(1.0f)
		>> ES::FadeOutBgm(1.0f)
		>> ES::Wait(1.0f)
		>> ES::ChangeLevel(Global::InteriorPlayersHouse2FLevel)
		>> ES::PlayBgm(RN::BgmPalletTown)
		>> ES::CameraFocus(Global::PlayerCharacter)
		>> ES::DeactivatePlayerControl()
		>> ES::FadeIn(1.0f)
		>> ES::FadeInBgm(1.0f)
		>> ES::Wait(1.0f)
		>> ES::End(true)
	);
}


void UTutorialLevel::LevelChange()
{
	UEventManager::TriggerEvent(MapLevelChanger);
}