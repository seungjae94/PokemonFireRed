#include "TitleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "TitleLevelManager.h"

UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay()
{
	UPokemonLevel::BeginPlay();

	UEngineDirectory CurDir;
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("TitleLevel");

	// 영상 로드
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("1st"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("2nd"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("3rd"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("4th"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("5th"));

	// 액터 생성
	Manager = SpawnActor<ATitleLevelManager>();
	Manager->SetActorLocation(Global::HalfScreen);

	UEventTargetInit LevelChangerInit; 
	LevelChangerInit.SetName("LevelChanger");
	UEventCondition LevelChangerCond;
	LevelChanger = SpawnEventTrigger<AEventTrigger>(LevelChangerInit);
	UEventManager::RegisterEvent(LevelChanger, LevelChangerCond,
		ES::Start(false)
		>> ES::FadeOut(1.0f) // White
		>> ES::ChangeLevel(Global::TutorialLevel)
		>> ES::FadeIn(0.5f)
		>> ES::End(false)
	);
}


void UTitleLevel::LevelChange()
{
	Manager->SetActive(false);
	UEventManager::TriggerEvent(LevelChanger);
}