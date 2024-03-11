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

	// (디버깅) 플레이어 시작 위치 설정
	UEventManager::SetPoint(GetName(), Global::Player, { 5, 5 });
	UEventManager::SetDirection(GetName(), Global::Player, FTileVector::Up);

	MakeDoor();
	MakeNurse();

	// 이벤트 렌더링에 필요한 하위 요소 세팅
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
	
	// 맵 레벨에서 직접 포켓몬 센터에 들어온 경우
	if (nullptr != MapLevel)
	{
		if (MapLevel->GetName() == UEngineString::ToUpper(Global::ExteriorPalletTownLevel))
		{
			SetDoorTargetAsViridianCity();
		}
		return;
	}
	
	// 배틀에서 패배해서 포켓몬 센터로 온 경우
	SetDoorTargetAsViridianCity(); // 임시로 상록 시티로 돌아오도록 설정
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
	Canvas->SetOptionBoxActive(false);
	Canvas->DecCursor();
	MsgBox->SetActive(true);
	MsgBox->SetMessage(L"Welcome to our POKéMON CENTER!");
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
		MsgBox->SetMessage(L"Would you like me to heal your\nPOKéMON back to perfect health?");
		MsgBox->Write();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealSelectMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = EState::HealSelect;
		Canvas->SetOptionBoxActive(true);
	}
}

void UInteriorPokemonCenterLevel::ProcessHealSelect()
{
	// 선택
	if (true == UEngineInput::IsDown('Z'))
	{
		int Cursor = Canvas->GetCursor();

		// 회복 선택
		if (Cursor == 0)
		{
			State = EState::YesHealMessage;
			Canvas->SetActive(false);
			MsgBox->SetMessage(L"Okay, I'll take your POKéMON for a\nfew seconds.");
			MsgBox->Write();
		}
		// 취소 선택
		else if (Cursor == 1)
		{
			State = EState::NoHealMessage;
			Canvas->SetActive(false);
			MsgBox->SetMessage(L"We hope to see you again!");
			MsgBox->Write();
		}
		return;
	}

	// 취소
	if (true == UEngineInput::IsDown('X'))
	{
		State = EState::NoHealMessage;
		Canvas->SetActive(false);
		MsgBox->SetMessage(L"We hope to see you again!");
		MsgBox->Write();
		return;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		Canvas->DecCursor();
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		Canvas->IncCursor();
	}
}

void UInteriorPokemonCenterLevel::ProcessNoHealMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState()
		&& true == UEngineInput::IsDown('Z'))
	{
		State = EState::CheckHealEvent;
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
	}
}

void UInteriorPokemonCenterLevel::ProcessYesHealMessage()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		int a = 0;
	}
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
