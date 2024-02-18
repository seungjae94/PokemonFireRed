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
	CurDir.MoveParent();
	CurDir.Move("Resources");
	CurDir.Move("TutorialLevel");

	// ���ҽ� �ε�
	std::list<UEngineFile> Files = CurDir.AllFile({".png"}, false);
	for (UEngineFile& File : Files)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	// ���� ����
	Manager = SpawnActor<ATutorialLevelManager>();

	UEventTargetInit LevelChangerInit; 
	LevelChangerInit.SetName("LevelChanger");
	UEventCondition LevelChangerCond;
	LevelChanger = SpawnEventTrigger<AEventTrigger>(LevelChangerInit);
	UEventManager::RegisterEvent(LevelChanger, LevelChangerCond,
		ES::Start(false)
		>> ES::FadeOut(1.0f)
		>> ES::ChangeLevel(Global::InteriorPlayersHouse2FLevel)
		>> ES::FadeIn(0.5f)
		>> ES::End(true)
	);
}


void UTutorialLevel::LevelChange()
{
	Manager->SetActive(false);
	UEventManager::TriggerEvent(LevelChanger);
}