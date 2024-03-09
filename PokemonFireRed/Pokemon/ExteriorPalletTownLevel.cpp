#include "ExteriorPalletTownLevel.h"
#include "Global.h"
#include "ExteriorDoor.h"
#include "DialogueActor.h"
#include "Text.h"
#include "PlayerData.h"
#include "Pokemon.h"
#include "SoundManager.h"

UExteriorPalletTownLevel::UExteriorPalletTownLevel()
{
}

UExteriorPalletTownLevel::~UExteriorPalletTownLevel()
{
}

void UExteriorPalletTownLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 76, 136 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Down);

	// 태초마을 생성
	MakePalletTown();
}

void UExteriorPalletTownLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	if (nullptr == _PrevLevel)
	{
		// 디버그용
		USoundManager::PlayBgm(RN::BgmPalletTown);
	}
}

void UExteriorPalletTownLevel::MakePalletTown()
{
	MakePTOaksLabDoor();
	MakePTPlayersHouseDoor();
	MakePTRivalsHouseDoor();
	MakePTTechMan();
	MakePTGetStarterEventTriggers();
	MakePTOak();
	MakePTAnimatedTiles();
}

void UExteriorPalletTownLevel::MakePTOaksLabDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::OaksLabDoor);
	Setting.SetPoint({ 80, 147 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName("GreenDoor");
	Setting.SetHeight(1);

	AExteriorDoor* OaksLabDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	OaksLabDoor->SetTargetMapName(Global::InteriorOaksLabLevel);
	OaksLabDoor->SetTargetPoint({ 6, 12 });
	OaksLabDoor->SetMoveDirection(FTileVector::Up);
	OaksLabDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTPlayersHouseDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::PlayersHouseDoor);
	Setting.SetPoint({ 70, 141 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName("RedDoor");
	Setting.SetHeight(1);

	AExteriorDoor* PlayersHouseDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	PlayersHouseDoor->SetTargetMapName(Global::InteriorPlayersHouse1FLevel);
	PlayersHouseDoor->SetTargetPoint({ 3, 8 });
	PlayersHouseDoor->SetMoveDirection(FTileVector::Up);
	PlayersHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTRivalsHouseDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::RivalshouseDoor);
	Setting.SetPoint({ 79, 141 });
	Setting.SetDirection(FTileVector::Up);
	Setting.SetImageName("RedDoor");
	Setting.SetHeight(1);

	AExteriorDoor* RivalsHouseDoor = SpawnEventTrigger<AExteriorDoor>(Setting);
	RivalsHouseDoor->SetTargetMapName(Global::InteriorRivalsHouseLevel);
	RivalsHouseDoor->SetTargetPoint({ 4, 8 });
	RivalsHouseDoor->SetMoveDirection(FTileVector::Up);
	RivalsHouseDoor->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTTechMan()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::TechMan);
	Setting.SetPoint({ 80, 151 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	ADialogueActor* TechMan = SpawnEventTrigger<ADialogueActor>(Setting);
	TechMan->SetDialogue({
		LR"(Technology is incredible!)",
		LR"(You can now store and recall items
and POKéMON as data via PC.)"
		});
	TechMan->SetTextColor(EFontColor::Blue);
	TechMan->RegisterPredefinedEvent();
}

void UExteriorPalletTownLevel::MakePTGetStarterEventTriggers()
{
	UEventCondition Cond = UEventCondition(EEventTriggerAction::StepOn);
	CheckFunc Func = []() {
		return false == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart);
		};
	Cond.RegisterCheckFunc(Func);

	UEventTargetSetting Setting0;
	Setting0.SetName(EN::GetStarterEventTrigger + "0");
	Setting0.SetPoint({ 76, 135 });

	std::vector<FTileVector> OakComePath0 = { Up, Up, Up, Up, Up, Right, Up };
	std::vector<FTileVector> OakGoToLabPath0 = { Down, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right, Right };
	std::vector<FTileVector> PlayerGoToLabPath0 = { Down, Down, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right };

	SpawnPTGetStarterEventTrigger(Setting0, Cond, OakComePath0, OakGoToLabPath0, PlayerGoToLabPath0);

	UEventTargetSetting Setting1;
	Setting1.SetName(EN::GetStarterEventTrigger + "1");
	Setting1.SetPoint({ 77, 135 });

	std::vector<FTileVector> OakComePath1 = { Up, Up, Up, Up, Up, Right, Right, Up };
	std::vector<FTileVector> OakGoToLabPath1 = { Down, Left, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right, Right };
	std::vector<FTileVector> PlayerGoToLabPath1 = { Down, Down, Left, Left, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Down, Right, Right, Right, Right };
	
	SpawnPTGetStarterEventTrigger(Setting1, Cond, OakComePath1, OakGoToLabPath1, PlayerGoToLabPath1);
}

void UExteriorPalletTownLevel::SpawnPTGetStarterEventTrigger(UEventTargetSetting _Setting, UEventCondition _Cond, const std::vector<FTileVector>& _OakComePath, const std::vector<FTileVector>& _OakGoToLabPath, const std::vector<FTileVector>& _PlayerGoToLabPath)
{
	AEventTrigger* Trigger = SpawnEventTrigger<AEventTrigger>(_Setting);
	UEventManager::RegisterEvent(Trigger, _Cond,
		ES::Start(true)
		>> ES::SetActive(GetName(), EN::Oak, true)
		>> ES::Chat({ L"OAK: Hey! Wait!\nDon't go out!" }, EFontColor::Blue, 16)
		>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, EN::Player, FTileVector::Down)
		//>> ES::Surprise(EN::Player, {})
		>> ES::Move(EN::Oak, _OakComePath, 3.6f, false)
		>> ES::Chat({
	   L"OAK: It's unsafe!\nWild POKéMON live in tall grass!",
	   L"You need your own POKéMON for\nyour protection.",
	   L"I know!\nHere, come with me!" }, EFontColor::Blue, 16)
	   >> ES::Move({ EN::Oak, EN::Player }, { _OakGoToLabPath, _PlayerGoToLabPath })
		>> ES::ChangeDirection(Global::ExteriorPalletTownLevel, EN::Oak, Up)
		>> ES::PlayAnimation(EN::OaksLabDoor, "DoorOpen")
		>> ES::Move({ EN::Oak, EN::Player }, { {Up}, {Right} })
		>> ES::SetActive(GetName(), EN::Oak, false)
		>> ES::Move(EN::Player, { Up })
		>> ES::HideActor(Global::Player)
		>> ES::PlayAnimation(EN::OaksLabDoor, "DoorClose")
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::SetActive(Global::InteriorOaksLabLevel, EN::Oak, true)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::Oak, { 6, 11 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::Oak, Up)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::Player, { 6, 12 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::Player, Up)
		>> ES::ChangeLevel(Global::InteriorOaksLabLevel)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::Move(EN::Oak, { Up, Up, Up, Up, Up })
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::Oak, { 6, 3 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::Oak, Down)
		>> ES::ChangePoint(Global::InteriorOaksLabLevel, EN::RivalGreen, { 5, 4 })
		>> ES::ChangeDirection(Global::InteriorOaksLabLevel, EN::RivalGreen, Up)
		>> ES::Move(EN::Player, { Up, Up, Up, Up, Up, Up, Up, Up })
		>> ES::Chat({ L"GREEN: Gramps!\nI'm fed up with waiting!" }, EFontColor::Blue, 16)
		>> ES::Wait(0.5f)
		>> ES::Chat({
		L"OAK: GREEN?\nLet me think...",
		L"Oh, that's right, I told you to\ncome! Just wait!",
		L"Here, RED.",
		L"There are three POKéMON here.",
		L"The POKéMON are held inside\nthese POKé BALLS.",
		L"You can have one.\nGo on, choose!"
			}, EFontColor::Blue, 16)
		>> ES::Achieve(EAchievement::GetStarterEventStart)
		>> ES::End(true)
	);
}

void UExteriorPalletTownLevel::MakePTOak()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::Oak);
	Setting.SetPoint({ 75, 142 });
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetWalkable(true);
	Setting.SetImageNameAuto();

	AEventTarget* Oak = SpawnEventTarget<AEventTarget>(Setting);
	Oak->SetActive(false);
}

void UExteriorPalletTownLevel::MakePTAnimatedTiles()
{
	DrawFlowers({
	{69, 146}, {69, 147},
	{70, 146}, {70, 147},
	{71, 146}, {71, 147},
	{72, 146}, {72, 147},
		});

	DrawSeas({
		{71, 151}, {72, 151}, {73, 151}, {74, 151},
		{71, 152}, {72, 152}, {73, 152}, {74, 152},
		{71, 153}, {72, 153}, {73, 153}, {74, 153},
		{71, 154}, {72, 154}, {73, 154}, {74, 154},
		{71, 155}, {72, 155}, {73, 155}, {74, 155},
		{71, 156}, {72, 156}, {73, 156}, {74, 156},
		{71, 157}, {72, 157}, {73, 157}, {74, 157},
		{71, 158}, {72, 158}, {73, 158}, {74, 158},
		});
}
