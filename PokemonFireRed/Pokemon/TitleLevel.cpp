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
}