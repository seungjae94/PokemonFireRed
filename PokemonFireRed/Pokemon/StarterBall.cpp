#include "StarterBall.h"
#include <EnginePlatform/EngineInput.h>
#include "PlayerData.h"
#include "EventManager.h"
#include "Player.h"
#include "DialogueWindow.h"

AStarterBall::AStarterBall()
{
}

AStarterBall::~AStarterBall()
{
}

void AStarterBall::SetPokemon(EPokedexNo _PokemonId)
{
}

void AStarterBall::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(ERenderingOrder::Lower);
	Renderer->SetImage(RN::MapBall);
	Renderer->SetTransform({ {0, 0}, {Global::FloatTileSize, Global::FloatTileSize} });

	Canvas = GetWorld()->SpawnActor<AStarterCanvas>();
	MsgBox = GetWorld()->SpawnActor<APokemonMsgBox>();

	MsgBox->SetBackgroundImage(RN::DialogueWindow);
	MsgBox->SetCoverImage(RN::DialogueWindowCover);
	MsgBox->SetTextColor(EFontColor::Gray);
	MsgBox->SetLineSpace(16);

	Canvas->SetActive(false);
	MsgBox->SetActive(false);
}

void AStarterBall::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	switch (State)
	{
	case AStarterBall::EState::None:
		CheckEventOccur();
		break;
	case AStarterBall::EState::NonEventMessage:
		ProcessNonEventMessage();
		break;
	case AStarterBall::EState::EventMessage1:
		break;
	case AStarterBall::EState::EventMessage2:
		break;
	case AStarterBall::EState::Select:
		break;
	case AStarterBall::EState::End:
		break;
	default:
		break;
	}
}

bool AStarterBall::IsZClickEventOccur()
{
	if (false == UEngineInput::IsDown('Z'))
	{
		return false;
	}

	APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(EN::Player);
	FTileVector PlayerPoint = Player->GetPoint();
	FTileVector PlayerDirection = Player->GetDirection();
	bool IsStaring = (PlayerPoint + PlayerDirection) == FTileVector(GetActorLocation());
	return IsStaring;
}

void AStarterBall::CheckEventOccur()
{
	if (true == IsZClickEventOccur())
	{
		// 스타팅 포켓몬 획득 이벤트가 아직 발생하지 않은 경우 
		if (false == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
		{
			// 대화창만 대충 띄운다.
			State = EState::NonEventMessage;
			MsgBox->SetActive(true);
			MsgBox->SetMessage(L"Those are POKé BALLS.\nThey contain POKéMON!");
			MsgBox->Write();
			UEventManager::DeactivatePlayer();
			return;
		}
		// 스타팅 포켓몬을 획득해야 하는 경우
		else if ((true == UPlayerData::IsAchieved(EAchievement::GetStarterEventStart))
			&& (false == UPlayerData::IsAchieved(EAchievement::GetFirstPokemon)))
		{
			// 스타팅 포켓몬 선택 대화창을 띄운다.
			OpenWhileEvent();
			return;
		}
		// 스타팅 포켓몬 획득 이후
		else
		{
			// 대화창만 대충 띄운다.
			State = EState::NonEventMessage;
			MsgBox->SetActive(true);
			MsgBox->SetMessage(L"That's PROF. OAK's last POKéMON");
			MsgBox->Write();
			UEventManager::DeactivatePlayer();
			return;
		}
	}
}

void AStarterBall::ProcessNonEventMessage()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd && true == UEngineInput::IsDown('Z'))
	{
		State = EState::None;
		UEventManager::ActivatePlayer();
		MsgBox->SetActive(false);
	}
}

void AStarterBall::OpenWhileEvent()
{
	Destroy();
}
