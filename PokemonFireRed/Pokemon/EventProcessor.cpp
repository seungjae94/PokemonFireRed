#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "EventTrigger.h"
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
			if (true == CurStream->DeactivatePlayer)
			{
				DeactivatePlayerControl();
			}
			IsRunningValue = true;		// 실행
			CurIndexOfTypeMap.clear();
			return true;
		}
	}

	return false;
}

void UEventProcessor::EndRun()
{
	if (true == CurStream->ActivatePlayer)
	{
		ActivatePlayerControl();
	}

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

	if (CurCommandIndex >= CurStream->EventTypeList.size())
	{
		MsgBoxAssert(Trigger->GetName() + "의 이벤트를 실행하는 중 에러가 발생했습니다. 이벤트 프로세서의 명령 인덱스가 이벤트 개수보다 크거나 같습니다. 이벤트 스트림은 항상 ES::End로 끝나야 합니다.");
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
	case EEventType::Move:
		ProcessingResult = ProcessMove();
		break;
	case EEventType::MoveWithoutRestriction:
		ProcessingResult = ProcessMoveWithoutRestriction();
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
	case EEventType::StarePlayer:
		ProcessingResult = ProcessStarePlayer();
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

void UEventProcessor::ActivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::Idle);
}

void UEventProcessor::DeactivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::GetCurPlayer();
	CurPlayer->StateChange(EPlayerState::OutOfControl);
}

bool UEventProcessor::ProcessMove()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::Move);
	ES::Move& Data = CurStream->MoveDataSet[CurIndexOfType];

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
	if (MovePathIndex == -1)
	{
		MoveTime = 1.0f / Data.MoveSpeed;
		MoveTimer = 0.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
		Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
	}

	if (MoveTimer > 0.0f)
	{
		MoveTimer -= DeltaTime;

		float t = (MoveTime - MoveTimer) / MoveTime;
		FVector TargetPos = UPokemonMath::Lerp(MovePrevPoint.ToFVector(), MoveNextPoint.ToFVector(), t);
		Target->SetActorLocation(TargetPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	}
	else if (MovePathIndex + 1 >= Data.Path.size())
	{
		// 이동 종료
		PostProcessMove(Target);
		return true;
	}
	else
	{
		UEventManager::SetPoint(MapName, TargetName, FTileVector(Target->GetActorLocation()));

		MovePrevPoint = Target->GetPoint();
		MoveNextPoint = MovePrevPoint + Data.Path[MovePathIndex + 1];
		MoveTimer = MoveTime;

		if (Data.Path[MovePathIndex + 1] == FTileVector::Zero)
		{
			MsgBoxAssert("MoveActor 함수에서 Path 값이 FTileVector::Zero 입니다.");
			return false;
		}

		if (Target->GetDirection() != Data.Path[MovePathIndex + 1])
		{
			Target->SetDirection(Data.Path[MovePathIndex + 1]);
			Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
		}
		MovePathIndex++;
	}
	return false;
}

void UEventProcessor::PostProcessMove(AEventTarget* _Target)
{
	MovePrevPoint = FTileVector::Zero;
	MoveNextPoint = FTileVector::Zero;
	MovePathIndex = -1;		// -1은 첫 번째 틱임을 의미한다.
	MoveTime = 0.0f;
	MoveTimer = 0.0f;
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeAnimation(_Target->GetMoveState(), _Target->GetDirection());
}

bool UEventProcessor::ProcessMoveWithoutRestriction()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::MoveWithoutRestriction);
	ES::MoveWithoutRestriction& Data = CurStream->MoveWithoutRestrictionDataSet[CurIndexOfType];

	std::string MapName = UEngineString::ToUpper(Data.MapName);
	std::string TargetName = UEngineString::ToUpper(Data.TargetName);

	if (Data.Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동(타일 제약 X) 경로의 크기가 0 이하입니다.");
		return false;
	}

	AEventTarget* Target = UEventManager::FindTarget(MapName, TargetName);
	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
		return false;
	}

	// 이동 이벤트 시작
	if (MoveWRPathIndex == -1)
	{
		MoveWRTime = 1.0f / Data.MoveSpeed;
		MoveWRTimer = 0.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
		Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
	}

	if (MoveWRTimer > 0.0f)
	{
		MoveWRTimer -= DeltaTime;

		float t = (MoveWRTime - MoveWRTimer) / MoveWRTime;
		FVector TargetPos = UPokemonMath::Lerp(MoveWRPrevPos, MoveWRNextPos, t);
		Target->SetActorLocation(TargetPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	}
	else if (MoveWRPathIndex + 1 >= Data.Path.size())
	{
		// 이동 종료
		PostProcessMoveWR(Target);
		return true;
	}
	else
	{
		MoveWRPrevPos = Target->GetActorLocation();
		MoveWRNextPos = MoveWRPrevPos + Data.Path[MoveWRPathIndex + 1];
		MoveWRTimer = MoveWRTime;

		if (true == Data.Path[MoveWRPathIndex + 1].IsZeroVector2D())
		{
			MsgBoxAssert("MoveActor 함수에서 Path 값이 FTileVector::Zero 입니다.");
			return false;
		}

		FTileVector ProjectedDirection = UPokemonMath::ProjectToTileVector(Data.Path[MoveWRPathIndex + 1]);
		if (Target->GetDirection() != ProjectedDirection)
		{
			Target->SetDirection(ProjectedDirection);
			Target->ChangeAnimation(Target->GetMoveState(), Target->GetDirection());
		}
		MoveWRPathIndex++;
	}
	return false;
}

void UEventProcessor::PostProcessMoveWR(AEventTarget* _Target)
{
	MoveWRPrevPos = FVector::Zero;
	MoveWRNextPos = FVector::Zero;
	MoveWRPathIndex = -1;		// -1은 첫 번째 틱임을 의미한다.
	MoveWRTime = 0.0f;
	MoveWRTimer = 0.0f;
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeAnimation(_Target->GetMoveState(), _Target->GetDirection());
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
	DeactivatePlayerControl();
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

bool UEventProcessor::ProcessStarePlayer()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::StarePlayer);
	ES::StarePlayer& Data = CurStream->StarePlayerDataSet[CurIndexOfType];

	std::string CurLevelName = UEventManager::GetCurLevelName();
	APlayer* Player = UEventManager::GetCurPlayer();
	UEventManager::SetDirection(CurLevelName, Data.TargetName, -Player->Direction);
	return true;
}
