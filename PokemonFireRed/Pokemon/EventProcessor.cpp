#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "Player.h"
#include "BlackScreen.h"
#include "DialogueWindow.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::RegisterStream(const UEventCondition& _Condition, UEventStream _Stream)
{
	AllStreams[_Condition] = _Stream;
}

bool UEventProcessor::TryRun(EEventTriggerAction _TriggerAction)
{
	for (std::pair<const UEventCondition, UEventStream>& Pair : AllStreams)
	{
		const UEventCondition& Condition = Pair.first;

		// 조건을 만족하고 현재 실행중이 아닐 때만 실행
		bool CheckResult = Condition.Check(_TriggerAction);
		if (true == CheckResult && false == IsRunningValue)
		{
			CurCommandIndex = 0;
			CurStream = &Pair.second;
			IsRunningValue = true;		// 실행
			CurIndexOfTypeMap.clear();
			return true;
		}
	}

	return false;
}

void UEventProcessor::EndRun()
{
	UEventManager::PlayerEventProcessingOff();
	IsRunningValue = false;
	CurIndexOfTypeMap.clear();
}

void UEventProcessor::Tick(float _DeltaTime)
{
	if (false == IsRunningValue)
	{
		return;
	}

	EEventType CurEventType = CurStream->EventTypeList[CurCommandIndex];

	if (CurEventType == EEventType::End)
	{
		EndRun();
		return;
	}

	bool ProcessingResult = false;
	switch (CurEventType)
	{
	case EEventType::ActivatePlayerControl:
		ProcessingResult = ProcessActivatePlayerControl();
		break;
	case EEventType::DeactivatePlayerControl:
		ProcessingResult = ProcessDeactivatePlayerControl();
		break;
	case EEventType::MoveActor:
		ProcessingResult = ProcessMoveActor();
		break;
	case EEventType::FadeIn:
		ProcessingResult = ProcessFadeIn();
		break;
	case EEventType::FadeOut:
		ProcessingResult = ProcessFadeOut();
		break;
	case EEventType::Wait:
		ProcessingResult = ProcessWait();
		break;
	case EEventType::Chat:
		ProcessingResult = ProcessChat();
		break;
	case EEventType::ChangeLevel:
		ProcessingResult = ProcessChangeLevel();
		break;
	case EEventType::ChangePoint:
		ProcessingResult = ProcessChangePoint();
		break;
	case EEventType::ChangeDirection:
		ProcessingResult = ProcessChangeDirection();
		break;
	default:
		break;
	}

	if (true == ProcessingResult)
	{
		IncCurIndexOfType(CurEventType);
		CurCommandIndex++;
	}
}

// 이벤트 명령 처리 함수

bool UEventProcessor::ProcessActivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::Idle);
	return true;
}

bool UEventProcessor::ProcessDeactivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::OutOfControl);
	return true;
}

bool UEventProcessor::ProcessMoveActor()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::MoveActor);
	ES::MoveActor& Data = CurStream->MoveActorDataSet[CurIndexOfType];

	std::string MapName = UEngineString::ToUpper(Data.MapName);
	std::string TargetName = UEngineString::ToUpper(Data.TargetName);

	if (Data.Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동 경로의 크기가 0 이하입니다.");
		return false;
	}

	AEventTarget* Target = UEventManager::FindTarget(MapName, TargetName);
	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
		return false;
	}

	// 이동 이벤트 시작
	if (Target->MoveIndex == -1)
	{
		Target->MoveTime = 1.0f / Data.MoveSpeed;
		Target->Timer = 0.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
		Target->ChangeAnimation(Target->MoveState, Target->Direction);
	}

	if (Target->Timer > 0.0f)
	{
		Target->Timer -= DeltaTime;

		float t = (Target->MoveTime - Target->Timer) / Target->MoveTime;
		FVector TargetPos = UPokemonMath::Lerp(Target->PrevPos, Target->NextPos, t);
		FVector PlayerPos = Target->GetActorLocation();
		FVector AddPos = TargetPos - PlayerPos;

		EngineDebug::OutPutDebugText("t: " + std::to_string(t));
		EngineDebug::OutPutDebugText("PlayerPos: " + PlayerPos.ToString());

		Target->AddActorLocation(AddPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HALF_SCREEN);
	}
	else if (Target->MoveIndex + 1 >= Data.Path.size())
	{
		Target->MoveIndex = -1;
		Target->SetMoveState(ETargetMoveState::Idle);
		Target->ChangeAnimation(Target->MoveState, Target->Direction);
		return true;
	}
	else
	{
		Target->PrevPos = Target->GetActorLocation();
		Target->NextPos = Target->PrevPos + Data.Path[Target->MoveIndex + 1].ToFVector();
		Target->Timer = Target->MoveTime;

		if (Data.Path[Target->MoveIndex + 1] == FTileVector::Zero)
		{
			MsgBoxAssert("MoveActor 함수에서 Path 값이 FTileVector::Zero 입니다.");
			return false;
		}

		if (Target->Direction != Data.Path[Target->MoveIndex + 1])
		{
			Target->SetDirection(Data.Path[Target->MoveIndex + 1]);
			Target->ChangeAnimation(Target->MoveState, Target->Direction);
		}
		Target->MoveIndex++;
	}
	return false;
}

bool UEventProcessor::ProcessFadeIn()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();
	std::string CurLevelName = UEventManager::GetCurLevelName();
	ABlackScreen* BlackScreen = UEventManager::GetBlackScreen(CurLevelName);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeIn);
	ES::FadeIn& Data = CurStream->FadeInDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
		BlackScreen->SetActiveRenderer(true);
		BlackScreen->SetAlpha(1.0f);
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		BlackScreen->SetActiveRenderer(false);
		return true;
	}

	Timer -= DeltaTime;
	BlackScreen->SetAlpha(Timer / Data.Time);
	return false;
}

bool UEventProcessor::ProcessFadeOut()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();
	std::string CurLevelName = UEventManager::GetCurLevelName();
	ABlackScreen* BlackScreen = UEventManager::GetBlackScreen(CurLevelName);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeOut);
	ES::FadeOut& Data = CurStream->FadeOutDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
		BlackScreen->SetActiveRenderer(true);
		BlackScreen->SetAlpha(0.0f);
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		BlackScreen->SetAlpha(1.0f);
		return true;
	}

	Timer -= DeltaTime;
	BlackScreen->SetAlpha((Data.Time - Timer) / Data.Time);
	return false;

}

bool UEventProcessor::ProcessWait()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();

	int CurIndexOfType = GetCurIndexOfType(EEventType::Wait);
	ES::Wait& Data = CurStream->WaitDataSet[CurIndexOfType];

	if (true == IsBegin)
	{
		Timer = Data.Time;
		IsBegin = false;
	}

	if (Timer <= 0.0f)
	{
		IsBegin = true;
		return true;
	}

	Timer -= DeltaTime;

	return false;
}

bool UEventProcessor::ProcessChat()
{
	ADialogueWindow* CurDialogueWindow = UEventManager::GetCurDialogueWindow();

	int CurIndexOfType = GetCurIndexOfType(EEventType::Chat);
	ES::Chat& Data = CurStream->ChatDataSet[CurIndexOfType];

	EDialogueWindowState State = CurDialogueWindow->GetState();
	
	if (State == EDialogueWindowState::End)
	{
		CurDialogueWindow->SetState(EDialogueWindowState::Hide);
		return true;
	}

	if (State == EDialogueWindowState::Show)
	{
		return false;
	}

	if (false == CurDialogueWindow->IsActive())
	{
		CurDialogueWindow->ActiveOn();
		CurDialogueWindow->AllRenderersActiveOn();
		CurDialogueWindow->SetDialogue(Data.Dialogue, Data.Color, Data.LineSpace, Data.IsSequential);
		return false;
	}

	return false;
}

bool UEventProcessor::ProcessChangeLevel()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangeLevel);
	ES::ChangeLevel& Data = CurStream->ChangeLevelDataSet[CurIndexOfType];
	UEventManager::SetLevel(Data.LevelName);
	return true;
}

bool UEventProcessor::ProcessChangePoint()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangePoint);
	ES::ChangePoint& Data = CurStream->ChangePointDataSet[CurIndexOfType];
	UEventManager::SetPoint(Data.LevelName, Data.TargetName, Data.Point);
	return true;
}

bool UEventProcessor::ProcessChangeDirection()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangeDirection);
	ES::ChangeDirection& Data = CurStream->ChangeDirectionDataSet[CurIndexOfType];
	UEventManager::SetDirection(Data.LevelName, Data.TargetName, Data.Direction);
	return true;
}