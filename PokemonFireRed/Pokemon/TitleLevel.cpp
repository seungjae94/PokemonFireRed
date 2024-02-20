#include "TitleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EngineBase/EngineTime.h>
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

	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("TitleLevel");

	// 액터 생성
	Manager = SpawnActor<ATitleLevelManager>();
	Manager->SetActorLocation(Global::HalfScreen);
	Manager->SetCurDir(&CurDir);

	UEventTargetInit LevelChangerInit; 
	LevelChangerInit.SetName("LevelChanger");
	UEventCondition LevelChangerCond;
	LevelChanger = SpawnEventTrigger<AEventTrigger>(LevelChangerInit);
	UEventManager::RegisterEvent(LevelChanger, LevelChangerCond,
		ES::Start(false)
		>> ES::FadeOut(1.0f)
		>> ES::Wait(1.0f)
		>> ES::ChangeLevel(Global::TutorialLevel)
		>> ES::FadeIn(1.0f)
		>> ES::Wait(1.0f)
		>> ES::End(false)
	);
}

void UTitleLevel::LevelChange()
{
	Manager->SetActive(false);
	UEventManager::TriggerEvent(LevelChanger);
}