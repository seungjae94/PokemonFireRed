#include "DialogueWindow.h"
#include <vector>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageRenderer.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonText.h"
#include "Global.h"

ADialogueWindow::ADialogueWindow()
{
}

ADialogueWindow::~ADialogueWindow()
{
}

void ADialogueWindow::AllRenderersActiveOn()
{
	DialogueWindowRenderer->ActiveOn();
	Text->SetVisible();
}

void ADialogueWindow::AllRenderersActiveOff()
{
	DialogueWindowRenderer->ActiveOff();
	Text->SetInvisible();
}

void ADialogueWindow::SetDialogue(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, bool _IsSequential)
{
	Dialogue = _Dialogue;
	Text->SetColor(_Color);
	Text->SetSequential(_IsSequential);
	Text->SetText(Dialogue[0]);
	State = EDialogueWindowState::Show;
}

void ADialogueWindow::BeginPlay()
{
	AActor::BeginPlay();

	DialogueWindowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	DialogueWindowRenderer->CameraEffectOff();
	DialogueWindowRenderer->SetImage("DialogueWindow.png");
	
	UWindowImage* DWImage = UEngineResourcesManager::GetInst().FindImg("DialogueWindow.png");
	FVector DWScale = DWImage->GetScale();
	FVector DWRenderScale = DWScale * Global::F_PIXEL_SIZE;
	FVector DWPos = DWRenderScale.Half2D() + FVector(0.0f, Global::F_SCREEN_Y - DWRenderScale.Y);
	DWPos += FVector(5.0f, -3.0f) * Global::F_PIXEL_SIZE;
	DialogueWindowRenderer->SetTransform({ DWPos, DWRenderScale});

	// 대화 내용
	Text = GetWorld()->SpawnActor<APokemonText>();
	Text->SetActorLocation(FVector(16, 132) * Global::F_PIXEL_SIZE);
	Text->SetText(LR"(Not Initialized...)");
}

void ADialogueWindow::Tick(float _DeltaTime)
{
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
			ActiveOff();
			AllRenderersActiveOff();
			return;
		}

		Text->SetText(Dialogue[Index]);
		return;
	}
}
