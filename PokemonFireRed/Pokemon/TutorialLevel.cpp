#include "TutorialLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "TutorialLevelManager.h"
#include "BlackScreen.h"

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

	std::list<UEngineFile> Sounds = CurDir.AllFile({ ".wav" }, false);
	for (UEngineFile& Sound : Sounds)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	// 액터 생성
	Manager = SpawnActor<ATutorialLevelManager>();

	UEventTargetInit LevelChangerInit; 
	LevelChangerInit.SetName("MapLevelChanger");
	UEventCondition LevelChangerCond;
	MapLevelChanger = SpawnEventTrigger<AEventTrigger>(LevelChangerInit);
	UEventManager::RegisterEvent(MapLevelChanger, LevelChangerCond,
		ES::Start(false)
		>> ES::FadeOut(1.0f)
		>> ES::Wait(1.0f)
		>> ES::ChangeLevel(Global::InteriorPlayersHouse2FLevel)
		>> ES::CameraFocus(Global::Player)
		>> ES::DeactivatePlayerControl()
		>> ES::FadeIn(1.0f)
		>> ES::Wait(1.0f)
		>> ES::End(true)
	);
}

void UTutorialLevel::LevelStart(ULevel* _PrevLevel)
{
	USoundManager::PlayBgm(RN::BgmTutorial);
}

void UTutorialLevel::LevelEnd(ULevel* _NextLevel)
{
	USoundManager::StopBgm();
}


void UTutorialLevel::LevelChange()
{
	UEventManager::TriggerEvent(MapLevelChanger);
}