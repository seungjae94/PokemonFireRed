#include "InteriorPewterGymLevel.h"
#include "InteriorDoor.h"
#include "Trainer.h"

UInteriorPewterGymLevel::UInteriorPewterGymLevel() 
{
}

UInteriorPewterGymLevel::~UInteriorPewterGymLevel() 
{
}

void UInteriorPewterGymLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 6, 14 });			// 회색시티 앞
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeCamper();
	MakeGymLeader();
	MakePewterCityDoor();
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
	Camper->SetPlayerWinMessage({ L"Darn!", L"Light-years isn't time…\nIt measures distance!"});
	Camper->AddPokemonToEntry(UPokemon(EPokemonId::Geodude, 10));
	Camper->AddPokemonToEntry(UPokemon(EPokemonId::Sandshrew, 11));

	UEventCondition Cond;
	AEventTrigger* WinTrigger = SpawnEventTrigger<AEventTrigger>("CamperWinTrigger");
	UEventManager::RegisterEvent(WinTrigger, Cond,
		ES::Start(false)
		>> ES::Achieve(EAchievement::FightWithPewterGymCamper)
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(Global::InteriorPewterGymLevel)
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
			>> ES::MoveDynamicPath(EN::Camper, CamperPathGenerator, Global::CharacterWalkSpeed, false)
			>> ES::Chat({ L"Stop right there, kid!", L"You're then thousand light-years\nfrom facing BROCK!"}, EFontColor::Blue, 16)
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

}

bool UInteriorPewterGymLevel::FightWithCamperChecker()
{
	return false == UPlayerData::IsAchieved(EAchievement::FightWithPewterGymCamper);
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