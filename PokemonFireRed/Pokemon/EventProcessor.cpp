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
			MsgBoxAssert(Trigger->GetName() + "�� �̺�Ʈ�� �����ϴ� �� ������ �߻��߽��ϴ�. �̺�Ʈ ���μ����� ��� �ε����� �̺�Ʈ �������� ũ�ų� �����ϴ�. �̺�Ʈ ��Ʈ���� �׻� ES::End�� ������ �մϴ�.");
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

// �̺�Ʈ ��� ó�� �Լ�

void UEventProcessor::ActivatePlayerControl()
{
	APlayerCharacter* CurPlayer = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);

	// �� ������ �ƴ� ��� �÷��̾��� ���¸� �ٲٷ��� ���� �ʴ´�.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::Idle);
}

void UEventProcessor::DeactivatePlayerControl()
{
	APlayerCharacter* CurPlayer = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);

	// �� ������ �ƴ� ��� �÷��̾��� ���¸� �ٲٷ��� ���� �ʴ´�.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::OutOfControl);
}