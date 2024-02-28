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
	
	// �����̳�(�޽��� �ڽ�)�� ���� ���� �� �ڵ����� ��� �ؽ�Ʈ�� ������ �� ���´�.
	ContainerElementSyncOff();

	DialogueWindowRenderer = CreateImageRenderer(ERenderingOrder::UI0);
	DialogueWindowRenderer->CameraEffectOff();
	DialogueWindowRenderer->SetImage("DialogueWindow.png");
	
	UWindowImage* DWImage = UEngineResourcesManager::GetInst().FindImg("DialogueWindow.png");
	FVector DWScale = DWImage->GetScale();
	FVector DWRenderScale = DWScale * Global::FloatPixelSize;
	FVector DWPos = DWRenderScale.Half2D() + FVector(0.0f, Global::FloatScreenY - DWRenderScale.Y);
	DWPos += UPokemonUtil::PixelVector(5, -3);
	DialogueWindowRenderer->SetTransform({ DWPos, DWRenderScale});

	// ��ȭ ����
	Text = CreateText(
		DialogueWindowRenderer,
		LR"(Not Initialized...)",
		EPivotType::LeftTop,
		EAlignType::Left,
		11, 17
	);
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

		// ��ȭ ����
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
