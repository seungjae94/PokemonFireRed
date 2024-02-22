#include "TutorialLevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "EventTrigger.h"
#include "EventManager.h"
#include "EventStream.h"
#include "PokemonMath.h"
#include "TutorialLevel.h"
#include "BlackScreen.h"

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
	Renderer->SetTransform({ Global::HalfScreen, Global::Screen });

	ArrowRenderer = CreateImageRenderer(ERenderingOrder::LowerUI);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("RedArrowDown.png");

	UWindowImage* ArrowImage = UEngineResourcesManager::GetInst().FindImg("RedArrowDown.png");
	FVector ArrowScale = ArrowImage->GetScale();
	ArrowDownPos = Global::Screen - ArrowScale.Half2D() - FVector(15, 15);
	ArrowRenderer->SetTransform({ArrowDownPos, ArrowScale});

	PikachuRenderer = CreateImageRenderer(ERenderingOrder::UpperUI);
	PikachuRenderer->CameraEffectOff();
	
	PikachuRenderer->SetImage(PikachuImageName);

	UWindowImage* PikachuImage = UEngineResourcesManager::GetInst().FindImg("StandEyeOpenEarClose.png");
	FVector PikachuScale = PikachuImage->GetScale();
	FVector PikachuRenderScale = PikachuScale * Global::FloatPixelSize;
	FVector PikachuPos = PikachuRenderScale.Half2D() + FVector(2, 1) * Global::FloatPixelSize;
	PikachuRenderer->SetTransform({ PikachuPos,  PikachuRenderScale });

	PikachuRenderer->SetActive(false);
}

void ATutorialLevelManager::Tick(float _DeltaTime)
{
	ABlackScreen* BlackScreen = UEventManager::FindCurLevelUIElement<ABlackScreen>("BlackScreen");

	if (FadingState == EFadeState::FadeOut)
	{
		CurFadeOutTime -= _DeltaTime;
		
		if (false == UiChanged && CurFadeOutTime < 0.0f)
		{
			// UI ����
			FVector ArrowScale = ArrowRenderer->GetTransform().GetScale();
			ArrowValue = 0.0f;
			IsArrowMoveUpward = true;
			ArrowDownPos = Global::Screen - ArrowScale.Half2D() - FVector(27, 27);
			ArrowRenderer->SetTransform({ ArrowDownPos, ArrowScale });
			PikachuRenderer->SetActive(true);
			Renderer->SetImage(GetPageName());
			UiChanged = true;
			return;
		}
		
		if (CurFadeOutTime + UIChangeWaitTime < 0.0f)
		{
			// ���� ����
			FadingState = EFadeState::FadeIn;
			BlackScreen->FadeIn(FadeInTime);
		}

		return;
	}

	if (FadingState == EFadeState::FadeIn)
	{
		CurFadeInTime -= _DeltaTime;
		if (CurFadeInTime < 0.0f)
		{
			FadingState = EFadeState::End;
		}
		return;
	}

	// ��ī�� �ִϸ��̼� ����
	// - ó������ ���ְ�, ���� �߰� �ְ�, �Ͱ� ���ִ� ����
	// - 0.5�� ���ִ� -> 0.5�� �ɾ��ִ� -> 0.5�� ���ִ� -> ... ������ �ݺ��Ѵ�.
	// - 3������ ���� 30/60�� �ĺ��� �� �����̴� ���� ����. ���� 206/60 ��, 322/60 ��, ... ���� ���� 2�� �����Ÿ���.
	// - �� �����̴� ���� = ���� 9/60�� ���´� -> 9/60�� ��� -> 9/60�� ���´� -> ���.
	// - 3������ ���� 60/60�� �ĺ��� �� �����̴� ���� ����. ���� 220/60 ��, 394/60 ��, ... ���� �͸� 2�� ������ ���.
	// - �� �����̴� ���� = �͸� 12/60�� ��ģ�� -> 12/60�� ���´� -> 12/60�� ��ģ�� -> ���´�.
	if (true == PageIndex >= 3)
	{
		CurCalcTime -= _DeltaTime;
		if (CurCalcTime <= 0.0f)
		{
			++CalcCount;
			CurCalcTime = CalcTime;
		}

		// ī��Ʈ�� �ö� ���� �̹����� �����Ѵ�.
		if (PrevCalcCount != CalcCount)
		{
			PrevCalcCount = CalcCount;

			if (CalcCount % 30 == 0)
			{
				IsStand = !IsStand;
			}

			// ���� �����ִ��� ���
			// - 30, 39, 48, 57, 236, 245, 254, 263
			// - 558, ...
			for (int Remainder : EyeChangeRemainders)
			{
				if (CalcCount % EyeChangeDivisor == Remainder)
				{
					IsEyeOpen = !IsEyeOpen;
					break;
				}
			}

			// �Ͱ� �����ִ��� ���
			// - 60, 72, 84, 96, 280, 292, 304, 316
			// - 674, ...
			for (int Remainder : EarChangeRemainders)
			{
				if (CalcCount % EarChangeDivisor == Remainder)
				{
					IsEarClose = !IsEarClose;
					break;
				}
			}

			// ��ī�� �̹��� �̸� ����
			std::string NewPikachuImageName = IsStand ? "Stand" : "Sit";
			NewPikachuImageName += IsEyeOpen ? "EyeOpen" : "EyeClose";
			NewPikachuImageName += IsEarClose ? "EarClose" : "EarOpen";
			NewPikachuImageName += ".png";

			if (NewPikachuImageName != PikachuImageName)
			{
				PikachuRenderer->SetImage(NewPikachuImageName);
				PikachuImageName = NewPikachuImageName;
			}
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
			// ���̵� ȿ�� ����
			FadingState = EFadeState::FadeOut;
			BlackScreen->FadeOut(FadeOutTime);
			return;
		}

		if (PageIndex >= 6)
		{
			// Ʃ�丮�� ���� ����
			UTutorialLevel* ThisLevel = dynamic_cast<UTutorialLevel*>(GetWorld());
			ThisLevel->LevelChange();
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
