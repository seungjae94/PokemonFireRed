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
	Balls = SpawnActor<APokemonCenterBalls>();
	
	MsgBox->SetBackgroundImage(RN::DialogueWindow);
	MsgBox->SetCoverImage(RN::DialogueWindowCover);
	MsgBox->SetTextColor(EFontColor::Red);
	MsgBox->SetLineSpace(16);

	Balls->SetActorLocation(FTileVector(5, 1).ToFVector() + FVector(1.0f/3, 1.0f/4) * Global::FloatTileSize);

	Canvas->SetActive(false);
	MsgBox->SetActive(false);
}

void UInteriorPokemonCenterLevel::Tick(float _DeltaTime)
{
	UMapLevel::Tick(_DeltaTime);

	Timer -= _DeltaTime;

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
	case EState::HealEndMessage3:
		ProcessHealEndMessage3();
		break;
	case EState::HealEndMessage4:
		ProcessHealEndMessage4();
		break;
	case EState::HealEndMessage5:
		ProcessHealEndMessage5();
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
		else if (MapLevel->GetName() == UEngineString::ToUpper(Global::ExteriorPewterCityLevel))
		{
			SetDoorTargetAsPewterCity();
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

	Nurse = SpawnEventTarget<AEventTarget>(Setting);
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

void UInteriorPokemonCenterLevel::SetDoorTargetAsPewterCity()
{
	Door->SetTargetMapName(Global::ExteriorPewterCityLevel);
	Door->SetTargetPoint({ 21, 30 });
	Door->SetMoveDirection(FTileVector::Down);
	Door->SetExteriorDoorName(EN::PewterPokemonCenterDoor);
	Door->SetTargetMapNameText(L"PEWTER CITY");
	Door->RegisterPredefinedEvent();
}

void UInteriorPokemonCenterLevel::ProcessCheckHealEvent()
{
	// 플레이어가 다른 이벤트를 실행중일 경우 이벤트를 체크하지 않는다.
	if (false == Player->HasControl())
	{
		return;
	}

	if (false == UEngineInput::IsDown('Z'))
	{
		return;
	}

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
		State = EState::BallAppear;
		Nurse->SetDirection(FTileVector::Left);
		MaxBallCount = UPlayerData::GetPokemonEntrySize();
		CurBallCount = 0;
		Timer = 0.0f;
	}
}

void UInteriorPokemonCenterLevel::ProcessBallAppear()
{
	if (Timer <= 0.0f)
	{
		Balls->SetActiveBall(CurBallCount, true);
		++CurBallCount;
		Timer = BallAppearInterval;

		if (CurBallCount == MaxBallCount)
		{
			State = EState::BallAnim;
			Balls->PlayHealAnimation();
			Timer = BallBlinkTime;
			return;
		}
	}

}

void UInteriorPokemonCenterLevel::ProcessBallAnim()
{
	if (Timer <= 0.0f)
	{
		State = EState::HealEndMessage1;
		Nurse->SetDirection(FTileVector::Down);
		Balls->StopHealAnimation();
		Balls->SetActiveAllBalls(false);
		MsgBox->SetMessage(L"Thank you for waiting.");
		MsgBox->Write();

		// 실제 치료 처리
		for (int i = 0; i < UPlayerData::GetPokemonEntrySize(); ++i)
		{
			UPokemon& Pokemon = UPlayerData::GetPokemonInEntry(i);
			Pokemon.HealAll();
			Pokemon.Cure();
		}
	}
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = EState::HealEndMessage2;
		MsgBox->ShowSkipArrow();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage2()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EState::HealEndMessage3;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"We've restored your POKeMON to\nfull health.");
		MsgBox->Write();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage3()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = EState::HealEndMessage4;
		MsgBox->ShowSkipArrow();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage4()
{
	if (true == UEngineInput::IsDown('Z'))
	{
		State = EState::HealEndMessage5;
		MsgBox->HideSkipArrow();
		MsgBox->SetMessage(L"We hope to see you again!");
		MsgBox->Write();
	}
}

void UInteriorPokemonCenterLevel::ProcessHealEndMessage5()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState()
		&& true == UEngineInput::IsDown('Z'))
	{
		State = EState::CheckHealEvent;
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
	}
}
