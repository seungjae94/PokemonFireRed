#include "TitleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineTime.h>
#include <EnginePlatform/EngineSound.h>
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

	// ���� ����
	Manager = SpawnActor<ATitleLevelManager>();
	Manager->SetCurDir(&CurDir);
}