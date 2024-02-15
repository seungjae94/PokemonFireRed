#include "TutorialLevelManager.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineResourcesManager.h>
#include "Global.h"
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

	UEventTargetInitialSetting Setting = UEventTargetInitialSetting("TutorialLevelEndTriger");
	UEventCondition Cond = UEventCondition(EEventTriggerAction::Direct);
	LevelEndTrigger = dynamic_cast<UTutorialLevel*>(GetWorld())->SpawnEventTrigger<AEventTrigger>(Setting);
	UEventManager::RegisterEvent(LevelEndTrigger, Cond, 
		ES::Start() >> ES::FadeOut(1.0f) >> ES::ChangeLevel("InteriorPlayersHouse2FLevel") >> ES::End()
	);
}

void ATutorialLevelManager::Tick(float _DeltaTime)
{
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
			// 화살표 위치 변경
			FVector ArrowScale = ArrowRenderer->GetTransform().GetScale();
			ArrowValue = 0.0f;
			IsArrowMoveUpward = true;
			ArrowDownPos = Global::SCREEN - ArrowScale.Half2D() - FVector(27, 27);
			ArrowRenderer->SetTransform({ ArrowDownPos, ArrowScale });
		}

		if (PageIndex >= 6)
		{
			// 튜토리얼 레벨 종료
			UEventManager::TriggerEvent(LevelEndTrigger);
			SetActive(false);
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
