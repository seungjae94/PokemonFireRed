#include "TutorialLevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "EventManager.h"
#include "PokemonMath.h"

ATutorialLevelManager::ATutorialLevelManager()
{
}

ATutorialLevelManager::~ATutorialLevelManager()
{
}

void ATutorialLevelManager::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	Renderer->CameraEffectOff();
	Renderer->SetImage(GetPageName());
	Renderer->SetTransform({ Global::HALF_SCREEN, Global::SCREEN });

	ArrowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("RedArrowDown.png");

	UWindowImage* ArrowImage = UEngineResourcesManager::GetInst().FindImg("RedArrowDown.png");
	FVector ArrowScale = ArrowImage->GetScale();
	ArrowDownPos = Global::SCREEN - ArrowScale.Half2D() - FVector(15, 15);
	ArrowRenderer->SetTransform({ArrowDownPos, ArrowScale});

}

void ATutorialLevelManager::Tick(float _DeltaTime)
{
	// Ʃ�丮�� ���� ���� ó��
	if (true == IsEnd)
	{
		bool FadeOutFinish = UEventManager::FadeOut(1.0f);

		if (true == FadeOutFinish)
		{
			UEventManager::ChangeLevel("InteriorPlayersHouse2FLevel");
			return;
		}
	}

	// ȭ��ǥ �̵�
	if (true == IsArrowMoveUpward)
	{
		ArrowValue += _DeltaTime;
		FVector NewPos = UPokemonMath::Lerp(ArrowDownPos, ArrowDownPos + ArrowMoveRange, ArrowValue);
		SetArrowPos(NewPos);

		if (ArrowValue >= MaxArrowValue)
		{
			IsArrowMoveUpward = false;
			ArrowValue = MaxArrowValue;
		}
	}
	else
	{
		ArrowValue -= _DeltaTime;
		FVector NewPos = UPokemonMath::Lerp(ArrowDownPos, ArrowDownPos + ArrowMoveRange, ArrowValue);
		SetArrowPos(NewPos);

		if (ArrowValue <= 0.0f)
		{
			IsArrowMoveUpward = true;
			ArrowValue = 0.0f;
		}
	}

	// ZŰ�� ���� ��� ���� �������� �� �� �ִٸ� ���� �������� ����.
	if (true == UEngineInput::IsDown('Z'))
	{
		PageIndex++;

		if (PageIndex == 3)
		{
			// ȭ��ǥ ��ġ ����
			FVector ArrowScale = ArrowRenderer->GetTransform().GetScale();
			ArrowValue = 0.0f;
			IsArrowMoveUpward = true;
			ArrowDownPos = Global::SCREEN - ArrowScale.Half2D() - FVector(27, 27);
			ArrowRenderer->SetTransform({ ArrowDownPos, ArrowScale });
		}

		if (PageIndex >= 6)
		{
			IsEnd = true;
			return;
		}

		Renderer->SetImage(GetPageName());
		return;
	}

	// XŰ�� ���� ��� ���� �������� �� �� �ִٸ� ���� �������� ����.
	if (true == UEngineInput::IsDown('X'))
	{
		if (PageIndex != 0 && PageIndex != 3)
		{
			PageIndex--;
			Renderer->SetImage(GetPageName());
		}
		return;
	}
}

std::string ATutorialLevelManager::GetPageName()
{
	return "TutorialPage" + std::to_string(PageIndex) + ".png";
}

void ATutorialLevelManager::SetArrowPos(const FVector& _Pos)
{
	FVector ArrowScale = ArrowRenderer->GetTransform().GetScale();
	ArrowRenderer->SetTransform({_Pos, ArrowScale});
}
