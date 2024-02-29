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

void ADialogueWindow::SetDialogue(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace, bool _IsSequential)
{
	Dialogue = _Dialogue;
	Text->SetColor(_Color);
	Text->SetLineSpace(_LineSpace);
	Text->SetSequential(_IsSequential);
	Text->SetText(Dialogue[0]);
	State = EDialogueWindowState::Show;

	IsFirstTick = true;
}

void ADialogueWindow::BeginPlay()
{
	ACanvas::BeginPlay();

	// 대화창
	MsgBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftBot, 0, 0);
	MsgBox->SetImage("DialogueWindow.png");

	// 대화 내용
	Text = CreateText(MsgBox, ERenderingOrder::UI1, EPivotType::LeftTop, 11, 17);
}

void ADialogueWindow::Tick(float _DeltaTime)
{
	ACanvas::Tick(_DeltaTime);

	if (true == IsFirstTick)
	{
		IsFirstTick = false;
		return;
	}

	if (true == UEngineInput::IsDown('Z') || true == UEngineInput::IsDown('X'))
	{
		if (false == Text->IsRenderEnd())
		{
			Text->Skip();
			return;
		}

		Index++;

		// 대화 종료
		if (Index >= Dialogue.size())
		{
			Index = 0;
			State = EDialogueWindowState::End;
			Dialogue.clear();
			SetActive(false);
			return;
		}

		Text->SetText(Dialogue[Index]);
		return;
	}
}
