#include "InteriorRivalsHouseLevel.h"
#include "InteriorDoor.h"
#include "DialogueActor.h"

UInteriorRivalsHouseLevel::UInteriorRivalsHouseLevel()
{
}

UInteriorRivalsHouseLevel::~UInteriorRivalsHouseLevel()
{
}

void UInteriorRivalsHouseLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 3, 3 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	// 이벤트 트리거 생성
	UEventTargetSetting PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 4, 9 });

	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 79, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("RivalsHouseDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();

	std::vector<std::wstring> DialogueSink =
	{
		LR"(It smells delicious!
			Somebody's been cooking here.)"
	};
	ADialogueActor* Sink0 = ADialogueActor::GenerateObject(this, "Sink0", { 0, 1 }, EFontColor::Gray, DialogueSink);
	ADialogueActor* Sink1 = ADialogueActor::GenerateObject(this, "Sink1", { 1, 1 }, EFontColor::Gray, DialogueSink);

	std::vector<std::wstring> DialogueDishDrawer =
	{
		LR"(Dishes and plates are neatly
			lined up.)"
	};
	ADialogueActor* DishDrawer0 = ADialogueActor::GenerateObject(this, "DishDrawer0", { 2, 1 }, EFontColor::Gray, DialogueDishDrawer);
	ADialogueActor* DishDrawer1 = ADialogueActor::GenerateObject(this, "DishDrawer1", { 3, 1 }, EFontColor::Gray, DialogueDishDrawer);

	std::vector<std::wstring> DialogueTV =
	{
		LR"(There's a POKéMON on TV!
			It looks like it's having fun.)",
	};
	ADialogueActor* TV = ADialogueActor::GenerateObject(this, "TV", { 5, 1 }, EFontColor::Gray, DialogueTV);

	std::vector<std::wstring> DialoguePic =
	{
		LR"(“The lovely and sweet
			CLEFAIRY”)",
	};
	ADialogueActor* Pic = ADialogueActor::GenerateObject(this, "Pic", { 9, 1 }, EFontColor::Gray, DialoguePic);


	std::vector<std::wstring> DialogueBookShelf =
	{
		LR"(The shelves are crammed full of
			books on POKéMON.)",
	};
	ADialogueActor* BookShelf0 = ADialogueActor::GenerateObject(this, "BookShelf0", { 11, 1 }, EFontColor::Gray, DialogueBookShelf);
	ADialogueActor* BookShelf1 = ADialogueActor::GenerateObject(this, "BookShelf1", { 12, 1 }, EFontColor::Gray, DialogueBookShelf);
}
