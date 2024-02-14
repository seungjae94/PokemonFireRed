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

	// ���ҽ� �ε�
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("1st"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("2nd"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("3rd"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("4th"));
	UEngineResourcesManager::GetInst().LoadFolder(CurDir.AppendPath("5th"));

	// ���� ����
	ATitleLevelManager* Manager = SpawnActor<ATitleLevelManager>();
	Manager->SetActorLocation(Global::HALF_SCREEN);
}
