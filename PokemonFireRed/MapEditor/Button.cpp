#include "Button.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCore.h>

AButton::AButton()
{
}

AButton::~AButton()
{
}

void AButton::BeginPlay()
{
	ButtonBackgroundRenderer = CreateImageRenderer(0);
	ButtonBackgroundRenderer->SetImage("ButtonBackground.png");
	ButtonBackgroundRenderer->SetTransform({ {0, 0}, {72, 72}});
	ButtonBackgroundRenderer->SetImageCuttingTransform({ {0, 0},  {512, 512} });

	ButtonHighLightRenderer = CreateImageRenderer(1);
	ButtonHighLightRenderer->SetImage("ButtonHighLight.png");
	ButtonHighLightRenderer->SetTransform({ {0, 0}, {72, 72} });
	ButtonHighLightRenderer->SetImageCuttingTransform({ {0, 0},  {512, 512} });
	ButtonHighLightRenderer->ActiveOff();

	IconRenderer = CreateImageRenderer(2);
	IconRenderer->SetImage("P_BlockDown.png");
	IconRenderer->SetTransform({ {0, 0}, {48, 48}});
	IconRenderer->SetImageCuttingTransform({ {0, 0},  {48, 48} });
}

void AButton::Tick(float _DeltaTime)
{
	if (true == UEngineInput::IsDown(VK_LBUTTON))
	{
		FVector MousePos = GEngine->MainWindow.GetMousePosition();
		FVector ButtonScale = { 64, 64 };
		FTransform ButtonAbsoluteTransform = { GetActorLocation(), ButtonScale};

		int ButtonLeft = ButtonAbsoluteTransform.iLeft();
		int ButtonTop = ButtonAbsoluteTransform.iTop();

		if (MousePos.iX() < ButtonLeft || MousePos.iX() >= ButtonLeft + ButtonScale.iX())
		{
			return;
		}

		if (MousePos.iY() < ButtonTop || MousePos.iY() >= ButtonTop + ButtonScale.iY())
		{
			return;
		}

		bool Active = ButtonHighLightRenderer->IsActive();
		
		if (true == Active)
		{
			ButtonHighLightRenderer->ActiveOff();
		}
		else 
		{
			ButtonHighLightRenderer->ActiveOn();
		}

	}
}
