#include "InteriorPewterGymLevel.h"
#include "InteriorDoor.h"
#include "Trainer.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>

UInteriorPewterGymLevel::UInteriorPewterGymLevel() 
{
}

UInteriorPewterGymLevel::~UInteriorPewterGymLevel() 
{
}

void UInteriorPewterGymLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	AreaBgm = RN::BgmGym;

	UEngineResourcesManager::GetInst().CuttingImage(RN::EndingRunner, 6, 1);

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 6, 14 });			// 회색시티 앞
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	Canvas = SpawnActor<AEndingCanvas>();
	Canvas->SetActive(false);

	MakeCamper();
	MakeGymLeader();
	MakePewterCityDoor();
}

void UInteriorPewterGymLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);

	switch (State)
	{
	case EState::None:
		if (true == UPlayerData::IsAchieved(EAchievement::FightWithPewterGymLeader) && true == Player->HasControl())
		{
			State = EState::Ending;
			UEventManager::DeactivatePlayer();
			Canvas->SetActive(true);
		}
		break;
	case EState::Ending:
		// 게임 종료 처리
		if (true == UEngineInput::IsDown('Z'))
		{
			GEngine->MainWindow.Off();
		}
		break;
	default:
		break;
	}
}

void UInteriorPewterGymLevel::MakePewterCityDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName("Door");
	Setting.SetPoint({ 6, 15 });

	AInteriorDoor* Door = SpawnEventTrigger<AInteriorDoor>(Setting);
	Door->SetTargetMapName(Global::ExteriorPewterCityLevel);
	Door->SetTargetPoint({ 19, 21 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::PewterGymDoor);
	Door->SetTargetMapNameText(L"PEWTER CITY");
	Door->SetTargetBgm(RN::BgmPewterCity);
	Door->RegisterPredefinedEvent();
}

void UInteriorPewterGymLevel::MakeCamper()
{
	int CamperX = 3;
	int CamperY = 8;

	UEventTargetSetting Setting;
	Setting.SetName(EN::Camper);
	Setting.SetPoint({ CamperX, CamperY });
	Setting.SetDirection(FTileVector::Right);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	ATrainer* Camper = SpawnEventTrigger<ATrainer>(Setting);
	Camper->SetBattler("CAMPER LIAM", RN::CamperBattler);
	Camper->SetPlayerWinMessage({ L"Darn!", L"Light-years isn't time\nIt measures distance!"});
	Camper->AddPokemonToEntry(UPokemon(EPokemonId::Diglett, 10));
	Camper->AddPokemonToEntry(UPokemon(EPokemonId::Sandshrew, 11));

	UEventCondition Cond;
	AEventTrigger* WinTrigger = SpawnEventTrigger<AEventTrigger>("CamperWinTrigger");
	UEventManager::RegisterEvent(WinTrigger, Cond,
		ES::Start(false)
		>> ES::Achieve(EAchievement::FightWithPewterGymCamper)
		>> ES::FadeOutBgm(0.5f)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(Global::InteriorPewterGymLevel)
		>> ES::PlayBgm(RN::BgmGym)
		>> ES::FadeInBgm(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	Camper->SetAfterBattleTrigger(WinTrigger);

	for (int i = 0; i < 4; ++i)
	{
		UEventTargetSetting BattleTileSetting;
		BattleTileSetting.SetName("CamperBattleTile" + std::to_string(i));
		BattleTileSetting.SetPoint({ CamperX + 1 + i, CamperY });

		UEventCondition BattleCond = UEventCondition(EEventTriggerAction::StepOn);
		BattleCond.RegisterCheckFunc(FightWithCamperChecker);

		AEventTrigger* BattleTile = SpawnEventTrigger<AEventTrigger>(BattleTileSetting);
		UEventManager::RegisterEvent(BattleTile, BattleCond,
			ES::Start(true)
			>> ES::PlayBgm(RN::BgmEncounterBoy)
			>> ES::Surprise(EN::Camper)
			>> ES::MoveDynamicPath(EN::Camper, CamperPathGenerator, Global::CharacterWalkSpeed, false)
			>> ES::Chat({ L"Stop right there, kid!", L"You're then thousand light-years\nfrom facing BROCK!"}, EFontColor::Blue, 16)
			>> ES::PlayBgm(RN::BgmTrainerBattle)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::FadeIn(0.15f)
			>> ES::Wait(0.15f)
			>> ES::FadeOut(0.25f)
			>> ES::Wait(0.25f)
			>> ES::TrainerBattle(Camper)
			>> ES::FadeIn(0.5f, EFadeType::VCurtain)
			>> ES::Wait(0.5f)
			>> ES::End(true)
		);
	}
}

void UInteriorPewterGymLevel::MakeGymLeader()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::LeaderBrock);
	Setting.SetPoint({ 6, 5 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(false);
	Setting.SetImageNameAuto();

	UEventCondition BattleCond = UEventCondition(EEventTriggerAction::ZClick);
	BattleCond.RegisterCheckFunc(FightWithBrockChecker);

	ATrainer* Brock = SpawnEventTrigger<ATrainer>(Setting);
	Brock->SetBattler("LEADER BROCK", RN::LeaderBrockBattler);
	Brock->SetPlayerWinMessage({ L"I took you for granted, and so\nlost.", L"As proof of your victory, I confer\non you this…", L"The official POKéMON LEAGUE\nBOULDERBADGE."});
	Brock->AddPokemonToEntry(UPokemon(EPokemonId::Geodude, 12));
	Brock->AddPokemonToEntry(UPokemon(EPokemonId::Onix, 14));
	Brock->SetGymLeader(true);

	UEventManager::RegisterEvent(Brock, BattleCond,
		ES::Start(true)
		>> ES::Chat({ L"So, you're here. I'm BROCK.\nI'm PEWTER's GYM LEADER.",
			L"My rock-hard willpower is evident\neven in my POKéMON.",
			L"My POKéMON are all rock hard, and\nhave true-grit determination.",
			L"That's right - my POKéMON are all\nthe ROCK type!",
			L"Fuhaha! You're going to challenge\nme knowing that you'll lose?",
			L"That's the TRAINER's honor that\ncompels you to challenge me." }, EFontColor::Blue, 16)
		>> ES::PlayBgm(RN::BgmEncounterGymLeader)
		>> ES::Chat({ L"Fine, then!\nShow me your best!" }, EFontColor::Blue, 16)
		>> ES::PlayBgm(RN::BgmGymLeaderBattle)
		>> ES::FadeOut(0.25f)
		>> ES::Wait(0.25f)
		>> ES::FadeIn(0.15f)
		>> ES::Wait(0.15f)
		>> ES::FadeOut(0.25f)
		>> ES::Wait(0.25f)
		>> ES::FadeIn(0.15f)
		>> ES::Wait(0.15f)
		>> ES::FadeOut(0.25f)
		>> ES::Wait(0.25f)
		>> ES::TrainerBattle(Brock)
		>> ES::FadeIn(0.5f, EFadeType::VCurtain)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	UEventCondition EndingCond;
	AEventTrigger* EndingTrigger = SpawnEventTrigger<AEventTrigger>("EndingTrigger");
	UEventManager::RegisterEvent(EndingTrigger, EndingCond,
		ES::Start(false)
		>> ES::Achieve(EAchievement::FightWithPewterGymLeader)
		>> ES::FadeOutBgm(0.5f)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(Global::InteriorPewterGymLevel)
		>> ES::PlayBgm(RN::BgmGym)
		>> ES::FadeInBgm(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);

	Brock->SetAfterBattleTrigger(EndingTrigger);
}

bool UInteriorPewterGymLevel::FightWithCamperChecker()
{
	return false == UPlayerData::IsAchieved(EAchievement::FightWithPewterGymCamper);
}

bool UInteriorPewterGymLevel::FightWithBrockChecker()
{
	return false == UPlayerData::IsAchieved(EAchievement::FightWithPewterGymLeader);
}

std::vector<FTileVector> UInteriorPewterGymLevel::CamperPathGenerator()
{
	const AEventTarget* Player = UEventManager::FindCurLevelTarget<AEventTarget>(EN::Player);
	const AEventTarget* Camper = UEventManager::FindCurLevelTarget<AEventTarget>(EN::Camper);

	int DiffX = Player->GetPoint().X - Camper->GetPoint().X;

	std::vector<FTileVector> Path;

	for (int i = 0; i < DiffX - 1; ++i)
	{
		Path.push_back(FTileVector::Right);
	}

	return Path;
}

void UInteriorPewterGymLevel::ProcessEnding()
{
}
