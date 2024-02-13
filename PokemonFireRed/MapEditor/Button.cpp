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
}

bool AButton::IsClicked(const FVector& _MousePos)
{
	FVector ButtonScale = { 64, 64 };
	FTransform ButtonAbsoluteTransform = { GetActorLocation(), ButtonScale };

	int ButtonLeft = ButtonAbsoluteTransform.iLeft();
	int ButtonTop = ButtonAbsoluteTransform.iTop();

	if (_MousePos.iX() < ButtonLeft || _MousePos.iX() >= ButtonLeft + ButtonScale.iX())
	{
		return false;
	}

	if (_MousePos.iY() < ButtonTop || _MousePos.iY() >= ButtonTop + ButtonScale.iY())
	{
		return false;
	}

	return true;
}

void AButton::SetIcon(std::string_view _IconName)
{
	IconRenderer = CreateImageRenderer(2);
	IconRenderer->SetImage(_IconName);
	IconRenderer->SetTransform({ {0, 0}, {48, 48} });
	IconRenderer->SetImageCuttingTransform({ {0, 0},  {48, 48} });
}

void AButton::ToggleHighlight()
{
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

void AButton::Tick(float _DeltaTime)
{
	
}
