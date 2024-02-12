#include "DialogueWindow.h"
#include <vector>
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

void ADialogueWindow::SetDialogues(const std::vector<std::wstring>& _Dialogues, EFontColor _Color)
{
	Text->SetColor(_Color);
	Text->SetLines(_Dialogues);
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
	Text->SetLines({
		L"This Text Is Not Initialized...",
	});
}

void ADialogueWindow::Tick(float _DeltaTime)
{
	this;
}
