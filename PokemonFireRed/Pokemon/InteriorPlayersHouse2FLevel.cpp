#include "InteriorPlayersHouse2FLevel.h"
#include <string>
#include "Stair.h"
#include "DialogueActor.h"
#include "EventStream.h"
#include "EventCondition.h"
#include "SoundManager.h"

UInteriorPlayersHouse2FLevel::UInteriorPlayersHouse2FLevel()
{
}

UInteriorPlayersHouse2FLevel::~UInteriorPlayersHouse2FLevel()
{
}

void UInteriorPlayersHouse2FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PlayerName, { 5, 6 });
	UEventManager::SetDirection(GetName(), Global::PlayerName, FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetInit StairTo1FSetting; 
	StairTo1FSetting.SetName("StairTo1F");
	StairTo1FSetting.SetPoint({ 8, 2 });

	AStair* StairTo1F = SpawnEventTrigger<AStair>(StairTo1FSetting);
	StairTo1F->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	StairTo1F->SetTargetPoint({ 9, 2 });
	StairTo1F->SetMoveDirection(FTileVector::Left);
	StairTo1F->SetFirstPath({FVector(-0.5f, 0.125f) * Global::FloatTileSize, FVector(-0.5f, 0.125f) * Global::FloatTileSize });
	StairTo1F->SetSecondPath({FVector(-0.5f, 0.25f) * Global::FloatTileSize, FVector(-0.5f, 0.25f) * Global::FloatTileSize });
	StairTo1F->RegisterPredefinedEvent();

	std::vector<std::wstring> DialogueNES =
	{
		L"RED played with the NES.",
		LR"(…Okay!
			It's time to go!)"
	};
	ADialogueActor* NES = ADialogueActor::GenerateObject(this, "NES", { 5, 5 }, EFontColor::Gray, DialogueNES);

	std::vector<std::wstring> DialogueDresser =
	{
		LR"(It's a nicely made dresser.
			It will hold a lot of stuff.)"
	};
	ADialogueActor* Dresser = ADialogueActor::GenerateObject(this, "Dresser", { 2, 1 }, EFontColor::Gray, DialogueDresser);

	std::vector<std::wstring> DialogueBookShelf =
	{
		LR"(It's crammed full of POKéMON
			books.)",
	};
	ADialogueActor* BookShelf0 = ADialogueActor::GenerateObject(this, "BookShelf0", { 3, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf1 = ADialogueActor::GenerateObject(this, "BookShelf1", { 4, 1 }, EFontColor::Gray, DialogueBookShelf);
}

void UInteriorPlayersHouse2FLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	if (nullptr == _PrevLevel || _PrevLevel->GetName() == UEngineString::ToUpper(Global::TutorialLevel))
	{
		USoundManager::PlayBgm(RN::BgmPalletTown);
	}
}
