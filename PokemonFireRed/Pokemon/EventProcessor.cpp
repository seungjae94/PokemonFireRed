#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "PlayerCharacter.h"
#include "DialogueWindow.h"
#include "MapNameCanvas.h"
#include "FadeCanvas.h"
#include "SoundManager.h"
#include "Trainer.h"
#include "Surprise.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::Run(AEventTrigger* _Trigger, const UEventStream* _Stream)
{
	Trigger = _Trigger;
	Stream = _Stream;
	IsRunningValue = true;
	CurCommandIndex = 0;

	if (true == Stream->DeactivatePlayer)
	{
		bIsPlayerActivated = false;
		DeactivatePlayerControl();
	}
}

void UEventProcessor::EndRun()
{
	ES::End* Data = dynamic_cast<ES::End*>(Stream->EventCommands[CurCommandIndex]);

	if (true == Data->ActivatePlayer)
	{
		ActivatePlayerControl();
	}

	IsRunningValue = false;
}

void UEventProcessor::Tick(float _DeltaTime)
{
	while (true)
	{
		if (false == IsRunningValue)
		{
			return;
		}

		if (CurCommandIndex >= Stream->EventCommands.size())
		{
			MsgBoxAssert(Trigger->GetName() + "의 이벤트를 실행하는 중 에러가 발생했습니다. 이벤트 프로세서의 명령 인덱스가 이벤트 개수보다 크거나 같습니다. 이벤트 스트림은 항상 ES::End로 끝나야 합니다.");
			return;
		}

		EEventType CurEventType = Stream->EventCommands[CurCommandIndex]->EventType;

		if (CurEventType == EEventType::End)
		{
			EndRun();
			return;
		}

		bool ProcessingResult = Stream->EventCommands[CurCommandIndex]->Process(_DeltaTime);

		if (true == ProcessingResult)
		{
			CurCommandIndex++;
		}
		else
		{
			return;
		}
	}
}

// 이벤트 명령 처리 함수

void UEventProcessor::ActivatePlayerControl()
{
	APlayerCharacter* CurPlayer = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);

	// 맵 레벨이 아닌 경우 플레이어의 상태를 바꾸려고 하지 않는다.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::Idle);
}

void UEventProcessor::DeactivatePlayerControl()
{
	APlayerCharacter* CurPlayer = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);

	// 맵 레벨이 아닌 경우 플레이어의 상태를 바꾸려고 하지 않는다.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::OutOfControl);
}