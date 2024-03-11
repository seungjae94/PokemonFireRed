#include "InteriorPokemonCenterLevel.h"
#include <EnginePlatform/EngineInput.h>
#include "InteriorDoor.h"
#include "PokemonMsgBox.h"
#include "PokemonCenterCanvas.h"

UInteriorPokemonCenterLevel::UInteriorPokemonCenterLevel() 
{
}

UInteriorPokemonCenterLevel::~UInteriorPokemonCenterLevel() 
{
}

void UInteriorPokemonCenterLevel::BeginPlay()
{
	UMapLevel::BeginPlay();

	// (�����) �÷��̾� ���� ��ġ ����
	UEventManager::SetPoint(GetName(), Global::Player, { 5, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeDoor();
	MakeNurse();

	// �̺�Ʈ �������� �ʿ��� ���� ��� ����
	Canvas = SpawnActor<APokemonCenterCanvas>();
	MsgBox = SpawnActor<APokemonMsgBox>();

	MsgBox->SetBackgroundImage(RN::DialogueWindow);
	MsgBox->SetCoverImage(RN::DialogueWindowCover);
	MsgBox->SetTextColor(EFontColor::Red);
	MsgBox->SetLineSpace(16);

	Canvas->SetActive(false);
	MsgBox->SetActive(false);
}

void UInteriorPokemonCenterLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);

	switch (State)
	{
	case EState::CheckHealEvent:
		ProcessCheckHealEvent();
		break;
	case EState::WelcomeMessage1:
		ProcessWelcomeMessage1();
		break;
	case EState::WelcomeMessage2:
		ProcessWelcomeMessage2();
		break;
	case EState::HealSelectMessage:
		ProcessHealSelectMessage();
		break;
	case EState::HealSelect:
		ProcessHealSelect();
		break;
	case EState::NoHealMessage:
		ProcessNoHealMessage();
		break;
	case EState::YesHealMessage:
		ProcessYesHealMessage();
		break;
	case EState::BallAppear:
		ProcessBallAppear();
		break;
	case EState::BallAnim:
		ProcessBallAnim();
		break;
	case EState::HealEndMessage1:
		ProcessHealEndMessage1();
		break;
	case EState::HealEndMessage2:
		ProcessHealEndMessage2();
		break;
	default:
		break;
	}
}

void UInteriorPokemonCenterLevel::LevelStart(ULevel* _PrevLevel)
{
	UMapLevel::LevelStart(_PrevLevel);

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(_PrevLevel);
	
	// �� �������� ���� ���ϸ� ���Ϳ� ���� ���
	if (nullptr != MapLevel)
	{
		if (MapLevel->GetName() == UEngineString::ToUpper(Global::ExteriorPalletTownLevel))
		{
			SetDoorTargetAsViridianCity();
		}
		return;
	}
	
	// ��Ʋ���� �й��ؼ� ���ϸ� ���ͷ� �� ���
	SetDoorTargetAsViridianCity(); // �ӽ÷� ��� ��Ƽ�� ���ƿ����� ����
}

void UInteriorPokemonCenterLevel::MakeDoor()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::InteriorDoor);
	Setting.SetPoint({ 7, 9 });

	Door = SpawnEventTrigger<AInteriorDoor>(Setting);
}

void UInteriorPokemonCenterLevel::MakeNurse()
{
	UEventTargetSetting Setting;
	Setting.SetName(EN::Nurse);
	Setting.SetPoint({7, 2});
	Setting.SetDirection(FTileVector::Down);
	Setting.SetCollidable(true);
	Setting.SetRotatable(true);
	Setting.SetImageNameAuto();

	AEventTarget* Nurse = SpawnEventTarget<AEventTarget>(Setting);
}

void UInteriorPokemonCenterLevel::SetDoorTargetAsViridianCity()
{
	Door->SetTargetMapName(Global::ExteriorPalletTownLevel);
	Door->SetTargetPoint({ 78, 81 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::ViridianPokemonCenterDoor);
	Door->SetTargetMapNameText(L"VIRIDIAN CITY");
	Door->RegisterPredefinedEvent();
}

void UInteriorPokemonCenterLevel::ProcessCheckHealEvent()
{
	if (false == UEngineInput::IsDown('Z'))
	{
		return;
	}

	APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(EN::Player);
	FTileVector PlayerPoint = Player->GetPoint();
	if (PlayerPoint != FTileVector(7, 4))
	{
		return;
	}

	FTileVector PlayerDirection = Player->GetDirection();
	if (PlayerDirection != FTileVector::Up)
	{
		return;
	}

	State = EState::WelcomeMessage1;
	UEventManager::DeactivatePlayer();
	Canvas->SetActive(true);
	MsgBox->SetActive(true);
	MsgBox->SetMessage(L"Welcome to our POKeMON CENTER!");
	MsgBox->Write();
}

void UInteriorPokemonCenterLevel::ProcessWelcomeMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = EState::WelcomeMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void UInteriorPokemonCenterLevel::ProcessWelcomeMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EState::HealSelectMessage;
		MsgBox->SetMessage(L"Would you like me to heal\nyour POKeMON back to perfect health?");
		MsgBox->Write();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealSelectMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{

		// Canvas->SetOptionBoxActive(true);
	}
}

void UInteriorPokemonCenterLevel::ProcessHealSelect()
{
	// Ŀ�� ����
}

void UInteriorPokemonCenterLevel::ProcessNoHealMessage()
{
}

void UInteriorPokemonCenterLevel::ProcessYesHealMessage()
{
}

void UInteriorPokemonCenterLevel::ProcessBallAppear()
{
}

void UInteriorPokemonCenterLevel::ProcessBallAnim()
{
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage1()
{
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage2()
{
}
