#include "InteriorOaksLabLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "EventMacros.h"
#include "InteriorDoor.h"
#include "DialogueActor.h"
#include "Trainer.h"
#include "StarterBall.h"
#include "SoundManager.h"
#include "EventStream.h"

UInteriorOaksLabLevel::UInteriorOaksLabLevel()
{
}

UInteriorOaksLabLevel::~UInteriorOaksLabLevel()
{
}

void UInteriorOaksLabLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	AreaBgm = RN::BgmOaksLab;

	// 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::PlayerCharacter, { 6, 6 });
	UEventManager::SetDirection(GetName(), Global::PlayerCharacter, FTileVector::Up);

	// 몬스터 볼 액터 구현
	// - 스타팅 포켓몬을 고를 때 대화창이 뜨는데 대화의 2번째 문장에 선택지를 넣어야 한다. 
	// - 기존 이벤트 시스템이 이 기능가지 지원하게 만들려면 너무 많은 작업이 필요하다. 
	// - 일관성을 잃더라도 직접 액터로 구현하는 것이 더 나아보인다.
	BulbasaurBall = SpawnActor<AStarterBall>();
	SquirtleBall = SpawnActor<AStarterBall>();
	CharmanderBall = SpawnActor<AStarterBall>();

	BulbasaurBall->SetActorLocation(FTileVector(8, 4).ToFVector());
	SquirtleBall->SetActorLocation(FTileVector(9, 4).ToFVector());
	CharmanderBall->SetActorLocation(FTileVector(10, 4).ToFVector());

	BulbasaurBall->SetPokemon(EPokemonId::Bulbasaur);
	SquirtleBall->SetPokemon(EPokemonId::Squirtle);
	CharmanderBall->SetPokemon(EPokemonId::Charmander);

	// 이벤트 트리거 생성
	MakeDoor();
	MakeOak();
	MakeRivalGreen();
	MakeAfterRivalBattleTrigger();
	MakeSpecialTriggers();
	MakeDecorations();
}

void UInteriorOaksLabLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);
}

void UInteriorOaksLabLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	// 디버그용 BGM 재생
	if (nullptr == _PrevLevel)
	{
		USoundManager::PlayBgm(RN::BgmOaksLab);
	}
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
	PalletTownDoor->SetTargetBgm(RN::BgmPalletTown);
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
	GreenInit.SetPoint({ 5, 4 });
	GreenInit.SetDirection(FTileVector::Down);
	GreenInit.SetCollidable(true);
	GreenInit.SetRotatable(true);
	GreenInit.SetWalkable(true);
	GreenInit.SetImageNameAuto();

	Green = SpawnEventTrigger<ATrainer>(GreenInit);
	Green->SetPlayerWinMessage({
		L"What?\nUnbelievable!",
		L"I picked the wrong POKéMON!"
		});
	Green->SetBattler("RIVAL GREEN", RN::RivalGreenBattler);
	Green->SetRegionGrass(false);

	// 이벤트 등록

	// 1. 스타팅 포켓몬 획득 이벤트가 아예 시작되기 전 상황 (대화 이벤트)
	std::function<bool()> BeforeGetStarterEventCond = []() {
		return false == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart);
	};

	Green->RegisterEvent( 
		EEventTriggerAction::ZClick,
		BeforeGetStarterEventCond,
		ES::Start(true)
		>> ES::StarePlayer(EN::RivalGreen)
		>> ES::Chat({ L"GREEN: What, it's only RED?\nGramps isn't around." }, EFontColor::Blue, 16)
		>> ES::End(true)
	);

	// 2. 스타팅 포켓몬 획득 이벤트는 시작했지만 아직 플레이어가 포켓몬을 고르지 않은 상황 (대화 이벤트)
	std::function<bool()> BeforePlayerSelectStarterCond = []() {
		return true == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart)
			&& false == UPlayerData::IsAchieved(EAchievement::SelectFirstPokemon);
	};

	Green->RegisterEvent( 
		EEventTriggerAction::ZClick,
		BeforePlayerSelectStarterCond,
		ES::Start(true)
		>> ES::StarePlayer(EN::RivalGreen)
		>> ES::Chat({ L"GREEN: Heh, I don't need to be\ngreedy like you. I'm mature!", L"Go ahead and choose, RED!" }, EFontColor::Blue, 16)
		>> ES::End(true)
	);

	// 3. 라이벌이 포켓몬을 고르는 이벤트
	std::function<bool()> PlayerSelectBulbasaurCond = []() {
		const UPokemon& PlayerSelectPokemon = UPlayerData::GetPokemonInEntry(0);
		return PlayerSelectPokemon.GetPokedexNo() == EPokemonId::Bulbasaur;
	};

	std::function<bool()> PlayerSelectSquirtleCond = []() {
		const UPokemon& PlayerSelectPokemon = UPlayerData::GetPokemonInEntry(0);
		return PlayerSelectPokemon.GetPokedexNo() == EPokemonId::Squirtle;
	};

	std::function<bool()> PlayerSelectCharmanderCond = []() {
		const UPokemon& PlayerSelectPokemon = UPlayerData::GetPokemonInEntry(0);
		return PlayerSelectPokemon.GetPokedexNo() == EPokemonId::Charmander;
	};

	Green->RegisterEvent(
		EEventTriggerAction::Direct,
		PlayerSelectBulbasaurCond,
		ES::Start(true)
		>> ES::Move(EN::RivalGreen, { Down, Down, Right, Right, Right, Right, Right, Up }, Global::CharacterWalkSpeed, false)
		>> ES::Chat({ L"GREEN: I'll take this one, then!" }, EFontColor::Blue, 16)
		>> ES::Destroy(CharmanderBall)
		>> ES::PlaySE(RN::SEPokemonObtained, 3.0f)
		>> ES::Chat({ L"GREEN received the CHARMANDER\nfrom PROF.OAK!" }, EFontColor::Gray, 16)
		>> ES::End(true)
	);

	Green->RegisterEvent(
		EEventTriggerAction::Direct, 
		PlayerSelectSquirtleCond,
		ES::Start(true)
		>> ES::Move(EN::RivalGreen, { Down, Down, Right, Right, Right, Up }, Global::CharacterWalkSpeed, false)
		>> ES::Chat({ L"GREEN: I'll take this one, then!" }, EFontColor::Blue, 16)
		>> ES::Destroy(BulbasaurBall)
		>> ES::PauseBgm()
		>> ES::PlaySE(RN::SEPokemonObtained, 3.0f)
		>> ES::Chat({ L"GREEN received the BULBASAUR\nfrom PROF.OAK!" }, EFontColor::Gray, 16)
		>> ES::End(true)
	);

	Green->RegisterEvent(
		EEventTriggerAction::Direct,
		PlayerSelectCharmanderCond,
		ES::Start(true)
		>> ES::Move(EN::RivalGreen, { Down, Down, Right, Right, Right, Right, Up }, Global::CharacterWalkSpeed, false)
		>> ES::Chat({ L"GREEN: I'll take this one, then!" }, EFontColor::Blue, 16)
		>> ES::Destroy(SquirtleBall)
		>> ES::PauseBgm()
		>> ES::PlaySE(RN::SEPokemonObtained, 3.0f)
		>> ES::Chat({ L"GREEN received the SQUIRTLE\nfrom PROF.OAK!" }, EFontColor::Gray, 16)
		>> ES::End(true)
	);

	// 라이벌이 포켓몬은 골랐지만 아직 배틀은 하지 않은 상황
	std::function<bool()> BeforeBattleCond = []() {
		return true == UPlayerData::IsAchieved(EAchievement::SelectFirstPokemon)
			&& false == UPlayerData::IsAchieved(EAchievement::FightWithGreen);
	};

	Green->RegisterEvent(
		EEventTriggerAction::ZClick,
		BeforeBattleCond,
		ES::Start(true)
		>> ES::StarePlayer(EN::RivalGreen)
		>> ES::Chat({ L"GREEN: My POKéMON looks a lot\ntougher than yours." }, EFontColor::Blue, 16)
		>> ES::End(true)
	);
}

/*
* 1. 스타팅 포켓몬 획득 이벤트는 발동했지만 스타팅 포켓몬을 고르지 않고 나가려고 하는 경우 막아주는 이벤트
* 2. 스타팅 포켓몬을 얻었지만 라이벌과 배틀을 하지 않았을 경우 라이벌과 배틀을 하는 이벤트
*/
void UInteriorOaksLabLevel::MakeSpecialTriggers()
{
	std::function<bool()> BlockCond = []() {
		return (true == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
			&& (false == UPlayerData::IsAchieved(EAchievement::SelectFirstPokemon));
	};

	std::function<bool()> RivalBattleCond = []() {
		return (true == UPlayerData::IsAchieved(EAchievement::SelectFirstPokemon))
			&& (false == UPlayerData::IsAchieved(EAchievement::FightWithGreen));
	};

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

void UInteriorOaksLabLevel::SpawnSpecialTrigger(
	UEventTargetSetting _Setting, 
	const std::function<bool()>& _BlockCond, 
	const std::function<bool()>& _RivalBattleCond)
{
	AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(_Setting);

	Trigger->RegisterEvent(
		EEventTriggerAction::StepOn,
		_BlockCond,
		ES::Start(true)
		>> ES::Chat({ L"OAK: Hey!\nDon't go away yet!" }, EFontColor::Blue, 16)
		>> ES::Move(EN::PlayerCharacter, {Up})
		>> ES::End(true)
	);

	Trigger->RegisterEvent(
		EEventTriggerAction::StepOn,
		_RivalBattleCond,
		ES::Start(true)
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::RivalGreen, Down)
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::PlayerCharacter, Up)
		>> ES::PlayBgm(RN::BgmRivalAppears)
		>> ES::Chat({ L"GREEN: Wait, RED!\nLet's check out our POKéMON!", L"Come on, I'll take you on!" }, EFontColor::Blue, 16)
		>> ES::MoveDynamicPath(EN::RivalGreen, BeforeRivalBattlePathGenerator, 3.6f, false)
		>> ES::PlayBgm(RN::BgmTrainerBattle)
		>> ES::Wait(0.25f)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::TrainerBattle(Green)
		>> ES::FadeIn(0.5f, EFadeType::VCurtain)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);
}

void UInteriorOaksLabLevel::MakeAfterRivalBattleTrigger()
{
	AfterRivalBattleTrigger = SpawnEventTrigger<AEventTrigger>(EN::AfterRivalBattleTrigger);
	Green->SetAfterBattleTrigger(AfterRivalBattleTrigger);

	Green->RegisterEvent(
		EEventTriggerAction::Direct,
		SKIP_CHECK,
		ES::Start(false)
		>> ES::FadeOutBgm(FadeOutTime)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::ChangeLevel(Global::InteriorOaksLabLevel)
		>> ES::DeactivatePlayerControl()
		>> ES::PlayBgm(RN::BgmOaksLab)
		>> ES::FadeInBgm(FadeInTime)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::Achieve(EAchievement::FightWithGreen)
		>> ES::Chat({L"GREEN: Okay! I'll make my\nPOKMON battle to toughen it up!", L"RED! Gramps!\nSmell you later!"}, EFontColor::Blue, 16)
		>> ES::MoveDynamicPath(EN::RivalGreen, AfterRivalBattlePathGenerator, Global::CharacterWalkSpeed, false)
		>> ES::Destroy(Green)
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

std::vector<FTileVector> UInteriorOaksLabLevel::BeforeRivalBattlePathGenerator()
{
	const APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);
	const ATrainer* Green = UEventManager::FindCurLevelTarget<ATrainer>(EN::RivalGreen);

	std::vector<FTileVector> DynamicPath;
	for (int i = 0; i < (Green->GetPoint().X - Player->GetPoint().X); ++i)
	{
		DynamicPath.push_back(FTileVector::Left);
	}
	DynamicPath.push_back(FTileVector::Down);
	DynamicPath.push_back(FTileVector::Down);
	return DynamicPath;
}

std::vector<FTileVector> UInteriorOaksLabLevel::AfterRivalBattlePathGenerator()
{
	const ATrainer* Green = UEventManager::FindCurLevelTarget<ATrainer>(EN::RivalGreen);
	FTileVector GreenPoint = Green->GetPoint();

	std::vector<FTileVector> DynamicPath;
	if (GreenPoint.X == 5)
	{
		DynamicPath.push_back(FTileVector::Right);
		for (int i = 0; i < 5; ++i)
		{
			DynamicPath.push_back(FTileVector::Down);
		}
	}
	else if (GreenPoint.X == 7)
	{
		DynamicPath.push_back(FTileVector::Left);
		for (int i = 0; i < 5; ++i)
		{
			DynamicPath.push_back(FTileVector::Down);
		}
	}
	else if (GreenPoint.X == 6)
	{
		DynamicPath.push_back(FTileVector::Right);
		for (int i = 0; i < 3; ++i)
		{
			DynamicPath.push_back(FTileVector::Down);
		}
		DynamicPath.push_back(FTileVector::Left);
		for (int i = 0; i < 2; ++i)
		{
			DynamicPath.push_back(FTileVector::Down);
		}
	}

	return DynamicPath;
}


