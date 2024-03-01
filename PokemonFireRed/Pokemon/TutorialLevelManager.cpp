#include "TutorialLevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
#include "PokemonUtil.h"
#include "EventTrigger.h"
#include "EventManager.h"
#include "EventStream.h"
#include "PokemonMath.h"
#include "TutorialLevel.h"

ATutorialLevelManager::ATutorialLevelManager()
{
}

ATutorialLevelManager::~ATutorialLevelManager()
{
}

void ATutorialLevelManager::BeginPlay()
{
	Renderer = CreateImageRenderer(ERenderingOrder::UI0);
	Renderer->CameraEffectOff();
	Renderer->SetImage(GetPageName());
	Renderer->SetTransform({ Global::HalfScreen, Global::Screen });

	ArrowRenderer = CreateImageRenderer(ERenderingOrder::UI0);
	ArrowRenderer->CameraEffectOff();
	ArrowRenderer->SetImage("RedArrowDown.png");

	UWindowImage* ArrowImage = UEngineResourcesManager::GetInst().FindImg("RedArrowDown.png");
	FVector ArrowScale = ArrowImage->GetScale();
	ArrowDownPos = Global::Screen - ArrowScale.Half2D() - FVector(15, 15);
	ArrowRenderer->SetTransform({ArrowDownPos, ArrowScale});

	PikachuRenderer = CreateImageRenderer(ERenderingOrder::UI2);
	PikachuRenderer->CameraEffectOff();
	
	PikachuRenderer->SetImage(PikachuImageName);

	UWindowImage* PikachuImage = UEngineResourcesManager::GetInst().FindImg("StandEyeOpenEarClose.png");
	FVector PikachuScale = PikachuImage->GetScale();
	FVector PikachuRenderScale = PikachuScale * Global::FloatPixelSize;
	FVector PikachuPos = PikachuRenderScale.Half2D() + UPokemonUtil::PixelVector(2, 1);
	PikachuRenderer->SetTransform({ PikachuPos,  PikachuRenderScale });

	PikachuRenderer->SetActive(false);
}

void ATutorialLevelManager::ChangeUI()
{
	FVector ArrowScale = ArrowRenderer->GetTransform().GetScale();
	ArrowValue = 0.0f;
	IsArrowMoveUpward = true;
	ArrowDownPos = Global::Screen - ArrowScale.Half2D() - FVector(27, 27);
	ArrowRenderer->SetTransform({ ArrowDownPos, ArrowScale });
	PikachuRenderer->SetActive(true);
	Renderer->SetImage(GetPageName());
	FadeCheckState = EFadeCheckState::End;
}

void ATutorialLevelManager::Tick(float _DeltaTime)
{
	if (FadeCheckState == EFadeCheckState::Start && Timer <= 0.0f)
	{
		ChangeUI();
	}

	// 피카츄 애니메이션 로직
	// - 처음에는 서있고, 눈을 뜨고 있고, 귀가 모여있는 형태
	// - 0.5초 서있다 -> 0.5초 앉아있다 -> 0.5초 서있다 -> ... 과정을 반복한다.
	// - 3페이지 진입 30/60초 후부터 눈 깜빡이는 로직 시작. 이후 206/60 초, 322/60 초, ... 마다 눈을 2번 깜빡거린다.
	// - 눈 깜빡이는 로직 = 눈을 9/60초 감는다 -> 9/60초 뜬다 -> 9/60초 감는다 -> 뜬다.
	// - 3페이지 진입 60/60초 후부터 귀 움직이는 로직 시작. 이후 220/60 초, 394/60 초, ... 마다 귀를 2번 접었다 편다.
	// - 귀 움직이는 로직 = 귀를 12/60초 펼친다 -> 12/60초 접는다 -> 12/60초 펼친다 -> 접는다.
	if (true == PageIndex >= 3)
	{
		CurCalcTime -= _DeltaTime;
		if (CurCalcTime <= 0.0f)
		{
			++CalcCount;
			CurCalcTime = CalcTime;
		}

		// 카운트가 올라갈 때만 이미지를 갱신한다.
		if (PrevCalcCount != CalcCount)
		{
			PrevCalcCount = CalcCount;

			if (CalcCount % 30 == 0)
			{
				IsStand = !IsStand;
			}

			// 눈이 떠져있는지 계산
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

			// 귀가 닫혀있는지 계산
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

			// 피카츄 이미지 이름 결정
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

	// 화살표 이동
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

	// Z키를 누를 경우 다음 페이지로 갈 수 있다면 다음 페이지로 간다.
	if (true == UEngineInput::IsDown('Z'))
	{
		PageIndex++;

		if (PageIndex == 3)
		{
			// 페이드 효과 시작
			AEventTrigger* Fader = UEventManager::FindCurLevelTarget<AEventTrigger>(Global::TutorialLevelFader);
			
			if (nullptr == Fader)
			{
				MsgBoxAssert("튜토리얼 레벨 페이더가 제대로 생성되지 않았습니다.");
				return;
			}

			UEventManager::TriggerEvent(Fader, EEventTriggerAction::Direct);
			FadeCheckState = EFadeCheckState::Start;
			return;
		}

		if (PageIndex >= 6)
		{
			// 튜토리얼 레벨 종료
			UTutorialLevel* ThisLevel = dynamic_cast<UTutorialLevel*>(GetWorld());
			ThisLevel->LevelChange();
			return;
		}

		Renderer->SetImage(GetPageName());
		return;
	}

	// X키를 누를 경우 이전 페이지로 갈 수 있다면 이전 페이지로 간다.
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
