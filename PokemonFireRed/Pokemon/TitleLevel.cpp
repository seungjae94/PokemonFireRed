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

	CurDir.MoveToSearchChild("Resources");
	CurDir.Move("TitleLevel");

	// ���� �ε�
	std::list<UEngineFile> AllSounds = CurDir.AllFile({ ".wav" });
	for (UEngineFile& Sound : AllSounds)
	{
		UEngineSound::Load(Sound.GetFullPath());
	}

	// ���� ����
	Manager = SpawnActor<ATitleLevelManager>();
	Manager->SetActorLocation(Global::HalfScreen);
	Manager->SetCurDir(&CurDir);
}