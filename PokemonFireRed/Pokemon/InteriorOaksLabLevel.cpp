#include "InteriorOaksLabLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "InteriorDoor.h"
#include "DialogueActor.h"

UInteriorOaksLabLevel::UInteriorOaksLabLevel()
{
}

UInteriorOaksLabLevel::~UInteriorOaksLabLevel()
{
}

void UInteriorOaksLabLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 6, 6 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);


	// 이벤트 트리거 생성
	UEventTargetInit PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 6, 13 });
	
	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 80, 148 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("OaksLabDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();

	std::vector<std::wstring> DialogueBigMachine =
	{
		LR"(What could this machine be?
			Better not mess around with it!)"
	};
	ADialogueActor* Machine0 = ADialogueActor::GenerateObject(this, "Machine0", { 1, 4 }, EFontColor::Gray, DialogueBigMachine);
	ADialogueActor* Machine1 = ADialogueActor::GenerateObject(this, "Machine1", { 1, 5 }, EFontColor::Gray, DialogueBigMachine);
	ADialogueActor* Machine2 = ADialogueActor::GenerateObject(this, "Machine2", { 2, 4 }, EFontColor::Gray, DialogueBigMachine);
	ADialogueActor* Machine3 = ADialogueActor::GenerateObject(this, "Machine3", { 2, 5 }, EFontColor::Gray, DialogueBigMachine);

	std::vector<std::wstring> DialogueSmallMachine =
	{
		LR"(Lights in different colors are
			flashing on and off.)"
	};
	ADialogueActor* Machine4 = ADialogueActor::GenerateObject(this, "Machine4", { 0, 1 }, EFontColor::Gray, DialogueSmallMachine);
	ADialogueActor* Machine5 = ADialogueActor::GenerateObject(this, "Machine5", { 1, 1 }, EFontColor::Gray, DialogueSmallMachine);

	std::vector<std::wstring> DialogueBookShelf =
	{
		LR"(It's crammed full of POKéMON
			books)"
	};
	ADialogueActor* BookShelf0 = ADialogueActor::GenerateObject(this, "BookShelf0", { 9, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf1 = ADialogueActor::GenerateObject(this, "BookShelf1", { 10, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf2 = ADialogueActor::GenerateObject(this, "BookShelf2", { 11, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf3 = ADialogueActor::GenerateObject(this, "BookShelf3", { 12, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf4 = ADialogueActor::GenerateObject(this, "BookShelf4", { 12, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf5 = ADialogueActor::GenerateObject(this, "BookShelf5", { 11, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf6 = ADialogueActor::GenerateObject(this, "BookShelf6", { 10, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf7 = ADialogueActor::GenerateObject(this, "BookShelf7", { 9, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf8 = ADialogueActor::GenerateObject(this, "BookShelf8", { 8, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf9 = ADialogueActor::GenerateObject(this, "BookShelf9", { 4, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf10 = ADialogueActor::GenerateObject(this, "BookShelf10", { 3, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf11 = ADialogueActor::GenerateObject(this, "BookShelf11", { 2, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf12 = ADialogueActor::GenerateObject(this, "BookShelf12", { 1, 8 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf13 = ADialogueActor::GenerateObject(this, "BookShelf13", { 0, 8 }, EFontColor::Gray, DialogueBookShelf);
}


