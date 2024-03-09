#include "InteriorOaksLabLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "InteriorDoor.h"
#include "DialogueActor.h"
#include "Trainer.h"

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
	MakeDoor();
	MakeOak();
	MakeRivalGreen();
	MakeSpecialTriggers();
	MakeDecorations();
}

void UInteriorOaksLabLevel::MakeDoor()
{
	UEventTargetSetting PalletTownDoorSetting;
	PalletTownDoorSetting.SetName("PalletTownDoor");
	PalletTownDoorSetting.SetPoint({ 6, 13 });

	AInteriorDoor* PalletTownDoor = SpawnEventTrigger<AInteriorDoor>(PalletTownDoorSetting);
	PalletTownDoor->SetTargetMapName(Global::ExteriorPalletTownLevel);
	PalletTownDoor->SetTargetPoint({ 80, 148 });
	PalletTownDoor->SetMoveDirection(FTileVector::Down);
	PalletTownDoor->SetExteriorDoorName("OaksLabDoor");
	PalletTownDoor->SetTargetMapNameText(L"PALLET TOWN");
	PalletTownDoor->RegisterPredefinedEvent();
}

void UInteriorOaksLabLevel::MakeOak()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::Oak);
	Setting.SetPoint({ -100, -100 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	AEventTarget* Oak = SpawnEventTarget<AEventTarget>(Setting);
	Oak->SetActive(false);
}

void UInteriorOaksLabLevel::MakeRivalGreen()
{
	UEventTargetSetting GreenInit;
	GreenInit.SetName("RIVALGREEN");
	GreenInit.SetPoint({ 10, 6 });
	GreenInit.SetDirection(FTileVector::Down);
	GreenInit.SetCollidable(true);
	GreenInit.SetRotatable(true);
	GreenInit.SetWalkable(false);						// 임시로 서있기만 가능한 캐릭터로 설정
	GreenInit.SetImageNameAuto();	// 임시로 서있기만 가능한 캐릭터로 설정

	UEventCondition GreenCond = UEventCondition(EEventTriggerAction::ZClick);
	ATrainer* Green = SpawnEventTrigger<ATrainer>(GreenInit);
	Green->AddPokemonToEntry(UPokemon(EPokedexNo::Rattata, 3));
	Green->AddPokemonToEntry(UPokemon(EPokedexNo::Charmander, 3));
	Green->SetPlayerWinMessage({
		L"Player win\nfirst message.",
		L"Player win\nsecond message."
		});
	Green->SetBattler("RIVAL GREEN", RN::RivalGreenBattler);

	UEventManager::RegisterEvent(Green, GreenCond,
		ES::Start(true)
		>> ES::Chat({ L"Let's fight!" }, EFontColor::Blue)
		>> ES::TrainerBattle(Green)
		>> ES::End(true)
	);
}

/*
* 1. 스타팅 포켓몬 획득 이벤트는 발동했지만 스타팅 포켓몬을 고르지 않고 나가려고 하는 경우 막아주는 이벤트
* 2. 스타팅 포켓몬을 얻었지만 라이벌과 배틀을 하지 않았을 경우 라이벌과 배틀을 하는 이벤트
*/
void UInteriorOaksLabLevel::MakeSpecialTriggers()
{
	UEventCondition BlockCond = UEventCondition(EEventTriggerAction::StepOn);
	CheckFunc BlockChecker = []() {
		return (true == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
			&& (false == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon));
	};
	BlockCond.RegisterCheckFunc(BlockChecker);

	UEventCondition RivalBattleCond = UEventCondition(EEventTriggerAction::StepOn);
	CheckFunc RivalBattleChecker = []() {
		return (true == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon))
			&& (false == UPlayerData::IsAchieved(EAchievement::FightWithGreen));
		};
	RivalBattleCond.RegisterCheckFunc(RivalBattleChecker);

	UEventTargetSetting Setting0;
	Setting0.SetName(EN::SpecialTrigger + "0");
	Setting0.SetPoint({ 5, 8 });

	UEventTargetSetting Setting1;
	Setting1.SetName(EN::SpecialTrigger + "1");
	Setting1.SetPoint({ 6, 8 });

	UEventTargetSetting Setting2;
	Setting2.SetName(EN::SpecialTrigger + "2");
	Setting2.SetPoint({ 7, 8 });

	SpawnSpecialTrigger(Setting0, BlockCond, RivalBattleCond);
	SpawnSpecialTrigger(Setting1, BlockCond, RivalBattleCond);
	SpawnSpecialTrigger(Setting2, BlockCond, RivalBattleCond);
}

void UInteriorOaksLabLevel::SpawnSpecialTrigger(UEventTargetSetting _Setting, UEventCondition _BlockCond, UEventCondition _RivalBattleCond)
{
	AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(_Setting);

	UEventManager::RegisterEvent(Trigger, _BlockCond,
		ES::Start(true)
		>> ES::Chat({ L"OAK: Hey!\nDon't go away yet!" }, EFontColor::Blue, 16)
		>> ES::Move(EN::Player, {Up})
		>> ES::End(true)
	);

	UEventManager::RegisterEvent(Trigger, _RivalBattleCond,
		ES::Start(true)
		>> ES::Chat({L"Rival Battle."}, EFontColor::Blue, 16)
		>> ES::End(true)
	);
}

void UInteriorOaksLabLevel::MakeDecorations()
{
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


