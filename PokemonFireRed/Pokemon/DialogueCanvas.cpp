#include "DialogueCanvas.h"
#include <vector>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageRenderer.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"
#include "Text.h"
#include "PokemonUtil.h"
#include "Global.h"

ADialogueCanvas::ADialogueCanvas()
{
}

ADialogueCanvas::~ADialogueCanvas()
{
}

void ADialogueCanvas::Open(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace, bool _IsSequential)
{
	Dialogue = _Dialogue;
	Text->SetColor(_Color);
	Text->SetLineSpace(_LineSpace);
	Text->SetSequential(_IsSequential);
	Text->SetText(Dialogue[0]);
	State = EDialogueCanvasState::Show;

	IsFirstTick = true;
}

void ADialogueCanvas::BeginPlay()
{
	ACanvas::BeginPlay();

	// 대화창
	MsgBox = CreateImageElement(this, ERenderingOrder::UI0, EPivotType::LeftBot, 5, 0);
	MsgBox->SetImage(RN::DialogueWindow);

	// 대화 내용
	Text = CreateText(MsgBox, ERenderingOrder::UI1, EPivotType::LeftTop, 11, 17);

	MsgBox->SetActive(false);
}

void ADialogueCanvas::Tick(float _DeltaTime)
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
			State = EDialogueCanvasState::End;
			Dialogue.clear();
			SetActive(false);
			return;
		}

		Text->SetText(Dialogue[Index]);
		return;
	}
}
