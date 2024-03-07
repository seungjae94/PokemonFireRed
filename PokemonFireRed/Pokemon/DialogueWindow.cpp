#include "DialogueWindow.h"
#include <vector>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageRenderer.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"
#include "Text.h"
#include "PokemonUtil.h"
#include "Global.h"

ADialogueWindow::ADialogueWindow()
{
}

ADialogueWindow::~ADialogueWindow()
{
}

void ADialogueWindow::SetActive(bool _Value, float _ActiveTime)
{
	AActor::SetActive(_Value, _ActiveTime);
	MsgBox->SetActive(_Value, _ActiveTime);
}

void ADialogueWindow::Open(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace)
{
	Dialogue = _Dialogue;
	Index = 0;

	MsgBox->SetBackgroundImage(RN::DialogueWindow);
	MsgBox->SetCoverImage(RN::DialogueWindowCover);
	MsgBox->SetTextColor(_Color);
	MsgBox->SetLineSpace(_LineSpace);
	MsgBox->SetMessage(Dialogue[0]);

	State = EState::Show;

	SetActive(true);
}

void ADialogueWindow::BeginPlay()
{
	AActor::BeginPlay();

	// 대화창
	MsgBox = GetWorld()->SpawnActor<APokemonMsgBox>();
}

void ADialogueWindow::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsPress('Z') || true == UEngineInput::IsPress('X'))
	{
		MsgBox->SetWriteSpeed(2.0f);
	}
	else
	{
		MsgBox->SetWriteSpeed(1.0f);
	}

	switch (State)
	{
	case ADialogueWindow::EState::Hide:
		break;
	case ADialogueWindow::EState::Show:
		ProcessShow();
		break;
	case ADialogueWindow::EState::End:
		break;
	default:
		break;
	}
}

void ADialogueWindow::ProcessShow()
{
	EWriteState MsgBoxState = MsgBox->GetWriteState();

	switch (MsgBoxState)
	{
	case EWriteState::None:
		MsgBox->Write();
		break;
	case EWriteState::Writing:
		break;
	case EWriteState::ScrollWaiting:
		if (true == UEngineInput::IsDown('Z'))
		{
			MsgBox->Write();
		}
		break;
	case EWriteState::Scrolling:
		break;
	case EWriteState::WriteEnd:
		// 화살표 표시
		MsgBox->ShowSkipArrow();
		if (true == UEngineInput::IsDown('Z'))
		{
			Index++;

			// 대화 종료
			if (Index >= Dialogue.size())
			{
				State = EState::End;
				return;
			}

			MsgBox->SetMessage(Dialogue[Index]);
			MsgBox->Write();
		}
		break;
	default:
		break;
	}
}
