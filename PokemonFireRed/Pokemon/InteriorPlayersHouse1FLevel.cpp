#include "InteriorPlayersHouse1FLevel.h"
#include "Stair.h"
#include "InteriorDoor.h"
#include "DialogueActor.h"
#include "Text.h"
#include "EventCondition.h"
#include "EventStream.h"

UInteriorPlayersHouse1FLevel::UInteriorPlayersHouse1FLevel()
{
}

UInteriorPlayersHouse1FLevel::~UInteriorPlayersHouse1FLevel()
{
}

void UInteriorPlayersHouse1FLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 7, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	// 이벤트 트리거 생성
	MakeStair();
	MakeDoor();
	MakePlayersMom();
	MakeDecorations();
}

void UInteriorPlayersHouse1FLevel::MakeStair()
{
	UEventTargetInit StairTo2FSetting;
	StairTo2FSetting.SetName("StairTo2F");
	StairTo2FSetting.SetPoint({ 10, 2 });

	AStair* StairTo2F = SpawnEventTrigger<AStair>(StairTo2FSetting);
	StairTo2F->SetTargetMapName(Global::InteriorPlayersHouse2FLevel);
	StairTo2F->SetTargetPoint({ 9, 2 });
	StairTo2F->SetMoveDirection(FTileVector::Right);
	StairTo2F->SetFirstPath({ FVector(0.5f, -0.25f) * Global::FloatTileSize, FVector(0.5f, -0.25f) * Global::FloatTileSize });
	StairTo2F->SetSecondPath({ FVector(0.5f, -0.125f) * Global::FloatTileSize, FVector(0.5f, -0.125f) * Global::FloatTileSize });
	StairTo2F->RegisterPredefinedEvent();
}

void UInteriorPlayersHouse1FLevel::MakeDoor()
{
	UEventTargetInit PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 3, 9 });

	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 70, 142 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("PlayersHouseDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();
}

void UInteriorPlayersHouse1FLevel::MakePlayersMom()
{
	UEventTargetInit PlayersMomSetting;
	PlayersMomSetting.SetName("PlayersMom");
	PlayersMomSetting.SetPoint({ 7, 4 });
	PlayersMomSetting.SetDirection(FTileVector::Left);
	PlayersMomSetting.SetCollidable(true);
	PlayersMomSetting.SetRotatable(true);
	PlayersMomSetting.SetWalkable(false);
	PlayersMomSetting.SetImageNameAuto();

	ADialogueActor* PlayersMom = SpawnEventTrigger<ADialogueActor>(PlayersMomSetting);
	PlayersMom->SetDialogue({
		LR"(MOM: …Right.
			All boys leave home someday.
			It said so on TV.)",
		LR"(Oh, yes. PROF. OAK, next door, was
			looking for you.)"
		});
	PlayersMom->SetTextColor(EFontColor::Red);
	PlayersMom->RegisterPredefinedEvent();
}

void UInteriorPlayersHouse1FLevel::MakeDecorations()
{
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
		LR"(There's a movie on TV.
			Four boys are walking on railroad
			tracks.)",
		LR"(...I better go, too.)"
	};
	ADialogueActor* TV = ADialogueActor::GenerateObject(this, "TV", { 5, 1 }, EFontColor::Gray, DialogueTV);
}
