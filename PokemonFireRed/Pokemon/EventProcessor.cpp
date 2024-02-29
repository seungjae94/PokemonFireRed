#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "Player.h"
#include "BlackScreen.h"
#include "DialogueCanvas.h"
#include "MapNameCanvas.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
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
	case EEventType::HideUI:
		ProcessingResult = ProcessHideUI();
		break;
	case EEventType::Wait:
		ProcessingResult = ProcessWait();
		break;
	case EEventType::PlayAnimation:
		ProcessingResult = ProcessPlayAnimation();
		break;
	case EEventType::Chat:
		ProcessingResult = ProcessChat();
		break;
	case EEventType::ShowMapName:
		ProcessingResult = ProcessShowMapName();
		break;
	case EEventType::ChangeLevel:
		ProcessingResult = ProcessChangeLevel();
		break;
	case EEventType::ChangePoint:
		ProcessingResult = ProcessChangePoint();
		break;
	case EEventType::ChangePosition:
		ProcessingResult = ProcessChangePosition();
		break;
	case EEventType::ChangeDirection:
		ProcessingResult = ProcessChangeDirection();
		break;
	case EEventType::StarePlayer:
		ProcessingResult = ProcessStarePlayer();
		break;
	case EEventType::HideActor:
		ProcessingResult = ProcessHideActor();
		break;
	case EEventType::ShowActor:
		ProcessingResult = ProcessShowActor();
		break;
	case EEventType::CameraFocus:
		ProcessingResult = ProcessCameraFocus();
		break;
	case EEventType::DeactivatePlayerControl:
		ProcessingResult = ProcessDeactivatePlayerControl();
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
	APlayer* CurPlayer = UEventManager::FindCurLevelTarget<APlayer>(Global::Player);

	// 맵 레벨이 아닌 경우 플레이어의 상태를 바꾸려고 하지 않는다.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::Idle);
}

void UEventProcessor::DeactivatePlayerControl()
{
	APlayer* CurPlayer = UEventManager::FindCurLevelTarget<APlayer>(Global::Player);

	// 맵 레벨이 아닌 경우 플레이어의 상태를 바꾸려고 하지 않는다.
	if (nullptr == CurPlayer)
	{
		return;
	}

	CurPlayer->StateChange(EPlayerState::OutOfControl);
}

bool UEventProcessor::ProcessMove()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::Move);
	ES::Move& Data = CurStream->MoveDataSet[CurIndexOfType];

	std::string MapName = ToUpper(Data.MapName);
	std::string TargetName = ToUpper(Data.TargetName);

	if (Data.Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동 경로의 크기가 0 이하입니다.");
		return false;
	}

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapName, TargetName);
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
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
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

		// 제자리 걸음이 아닐 경우 Path에 입력된 방향으로 타겟의 방향을 바꾼다.
		if (Data.Path[MovePathIndex + 1] != FTileVector::Zero && Target->GetDirection() != Data.Path[MovePathIndex + 1])
		{
			Target->SetDirection(Data.Path[MovePathIndex + 1]);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
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
	UEventManager::SetPoint(_Target->GetWorld()->GetName(), _Target->GetName(), FTileVector(_Target->GetActorLocation()));
	_Target->ResetFootOrder();
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeMoveAnimation(_Target->GetMoveState(), _Target->GetDirection());
}

bool UEventProcessor::ProcessMoveWithoutRestriction()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::MoveWithoutRestriction);
	ES::MoveWithoutRestriction& Data = CurStream->MoveWithoutRestrictionDataSet[CurIndexOfType];

	std::string MapName = ToUpper(Data.MapName);
	std::string TargetName = ToUpper(Data.TargetName);

	if (Data.Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동(타일 제약 X) 경로의 크기가 0 이하입니다.");
		return false;
	}

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapName, TargetName);
	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
		return false;
	}

	// 이동 이벤트 시작
	if (MoveWRPathIndex == -1)
	{
		MoveWRTimer = -1.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
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
		float PathSize = Data.Path[MoveWRPathIndex + 1].Size2D() / Global::FloatTileSize;
		MoveWRTime = PathSize / Data.MoveSpeed;

		MoveWRPrevPos = Target->GetActorLocation();
		MoveWRNextPos = MoveWRPrevPos + Data.Path[MoveWRPathIndex + 1];
		MoveWRTimer = MoveWRTime;

		// 제자리 걸음이 아닐 경우 Path에 입력된 벡터를 가장 가까운 좌표축에 투영하고 정규화한 벡터로 타겟의 방향을 바꾼다.
		FTileVector ProjectedDirection = UPokemonMath::ProjectToTileVector(Data.Path[MoveWRPathIndex + 1]);
		if (ProjectedDirection != FTileVector::Zero && Target->GetDirection() != ProjectedDirection)
		{
			Target->SetDirection(ProjectedDirection);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
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
	_Target->ResetFootOrder();
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeMoveAnimation(_Target->GetMoveState(), _Target->GetDirection());
}

bool UEventProcessor::ProcessFadeIn()
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	AFadeScreen* FadeScreen = nullptr;

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeIn);
	ES::FadeIn& Data = CurStream->FadeInDataSet[CurIndexOfType];

	switch (Data.FadeType)
	{
	case EFadeType::Black:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("BlackScreen");
		break;
	case EFadeType::White:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("WhiteScreen");
		break;
	case EFadeType::Curtain:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("CurtainScreen");
		break;
	default:
		MsgBoxAssert("아직 FadeIn 기능을 지원하지 않는 FadeType을 사용했습니다.");
		break;
	}

	FadeScreen->FadeIn(Data.Time);
	return true;
}

bool UEventProcessor::ProcessFadeOut()
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	AFadeScreen* FadeScreen = nullptr;

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeOut);
	ES::FadeOut& Data = CurStream->FadeOutDataSet[CurIndexOfType];

	switch (Data.FadeType)
	{
	case EFadeType::Black:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("BlackScreen");
		break;
	case EFadeType::White:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("WhiteScreen");
		break;
	case EFadeType::Curtain:
		FadeScreen = UEventManager::FindCurLevelMapLevelCanvas<AFadeScreen>("CurtainScreen");
		break;
	default:
		MsgBoxAssert("아직 FadeOut 기능을 지원하지 않는 FadeType을 사용했습니다.");
		break;
	}

	FadeScreen->FadeOut(Data.Time);
	return true;

}

bool UEventProcessor::ProcessHideUI()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::HideUI);
	ES::HideUI& Data = CurStream->HideUIDataSet[CurIndexOfType];

	ACanvas* Element = UEventManager::FindCurLevelMapLevelCanvas<ACanvas>(Data.ElementName);

	if (nullptr == Element)
	{
		MsgBoxAssert(Data.ElementName + "은 유효한 UI 엘리먼트 이름이 아닙니다.");
		return false;
	}

	Element->ActiveOff();
	Element->AllRenderersActiveOff();

	return true;
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

bool UEventProcessor::ProcessPlayAnimation()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::PlayAnimation);
	ES::PlayAnimation& Data = CurStream->PlayAnimationDataSet[CurIndexOfType];

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data.TargetName);

	if (true == PlayAnimIsFirstTick)
	{
		// 애니메이션 재생
		switch (Data.AnimTarget)
		{
		case EAnimTarget::UpperBodyOnly:
			Target->UpperBodyRenderer->ChangeAnimation(Data.AnimName + Global::SuffixUpperBody);
			break;
		case EAnimTarget::LowerBodyOnly:
			Target->LowerBodyRenderer->ChangeAnimation(Data.AnimName + Global::SuffixLowerBody);
			break;
		case EAnimTarget::All:
			if (1 == Target->Height)
			{
				Target->LowerBodyRenderer->ChangeAnimation(Data.AnimName);
			}
			else
			{
				Target->UpperBodyRenderer->ChangeAnimation(Data.AnimName + Global::SuffixUpperBody);
				Target->LowerBodyRenderer->ChangeAnimation(Data.AnimName + Global::SuffixLowerBody);
			}
			break;
		default:
			break;
		}

		if (false == Data.Wait)
		{
			// 비동기적 재생인 경우
			return true;
		}

		PlayAnimIsFirstTick = false;
		return false;
	}

	bool IsAnimEnd = false;

	switch (Data.AnimTarget)
	{
	case EAnimTarget::UpperBodyOnly:
		IsAnimEnd = Target->UpperBodyRenderer->IsCurAnimationEnd();
		break;
	case EAnimTarget::LowerBodyOnly:
		IsAnimEnd = Target->LowerBodyRenderer->IsCurAnimationEnd();
		break;
	case EAnimTarget::All:
		if (1 == Target->Height)
		{
			IsAnimEnd = Target->LowerBodyRenderer->IsCurAnimationEnd();
		}
		else
		{
			IsAnimEnd = Target->UpperBodyRenderer->IsCurAnimationEnd() && Target->LowerBodyRenderer->IsCurAnimationEnd();
		}
		break;
	default:
		break;
	}
	
	if (true == IsAnimEnd)
	{
		// 함수 상태 초기화 후 이벤트 종료
		PlayAnimIsFirstTick = true;
		return true;
	}

	return false;
}

bool UEventProcessor::ProcessChat()
{
	ADialogueCanvas* CurDialogueCanvas = UEventManager::FindCurLevelMapLevelCanvas<ADialogueCanvas>("DialogueCanvas");

	int CurIndexOfType = GetCurIndexOfType(EEventType::Chat);
	ES::Chat& Data = CurStream->ChatDataSet[CurIndexOfType];

	EDialogueCanvasState State = CurDialogueCanvas->GetState();

	if (State == EDialogueCanvasState::End)
	{
		CurDialogueCanvas->SetState(EDialogueCanvasState::Hide);
		return true;
	}

	if (State == EDialogueCanvasState::Show)
	{
		return false;
	}

	if (false == CurDialogueCanvas->IsActive())
	{
		CurDialogueCanvas->SetActive(true);
		CurDialogueCanvas->Open(Data.Dialogue, Data.Color, Data.LineSpace, Data.IsSequential);
		return false;
	}

	return false;
}

bool UEventProcessor::ProcessShowMapName()
{
	AMapNameCanvas* MapNameCanvas = UEventManager::FindCurLevelMapLevelCanvas<AMapNameCanvas>("MapNameCanvas");

	int CurIndexOfType = GetCurIndexOfType(EEventType::ShowMapName);
	ES::ShowMapName& Data = CurStream->ShowMapNameDataSet[CurIndexOfType];
	MapNameCanvas->SetActive(true);
	MapNameCanvas->AllRenderersActiveOn();
	MapNameCanvas->Open(Data.MapName);
	return true;
}

bool UEventProcessor::ProcessChangeLevel()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangeLevel);
	ES::ChangeLevel& Data = CurStream->ChangeLevelDataSet[CurIndexOfType];

	std::string PrevLevelName = UEventManager::CurLevelName;
	std::string NextLevelName = ToUpper(Data.LevelName);

	UEventManager::SetLevel(NextLevelName);
	if (false == IsPlayerActivated)
	{
		DeactivatePlayerControl();
	}

	// 여러 레벨의 페이드 스크린을 같은 상태로 만들기
	for (std::pair<const std::string, ACanvas*>& Pair 
		: UEventManager::AllMapLevelCanvas[PrevLevelName])
	{
		std::string PrevElementName = Pair.first;
		ACanvas* PrevElement = Pair.second;

		if (false == UEventManager::AllMapLevelCanvas[NextLevelName].contains(PrevElementName))
		{
			continue;
		}

		ACanvas* NextElement = UEventManager::AllMapLevelCanvas[NextLevelName][PrevElementName];
		NextElement->Sync(PrevElement);
	}

	return true;
}

bool UEventProcessor::ProcessChangePoint()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangePoint);
	ES::ChangePoint& Data = CurStream->ChangePointDataSet[CurIndexOfType];
	UEventManager::SetPoint(Data.LevelName, Data.TargetName, Data.Point);
	return true;
}

bool UEventProcessor::ProcessChangePosition()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ChangePosition);
	ES::ChangePosition& Data = CurStream->ChangePositionDataSet[CurIndexOfType];

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(Data.LevelName, Data.TargetName);
	Target->SetActorLocation(Data.Position);
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
	APlayer* Player = UEventManager::FindCurLevelTarget<APlayer>(Global::Player);
	UEventManager::SetDirection(CurLevelName, Data.TargetName, -Player->Direction);
	return true;
}

bool UEventProcessor::ProcessHideActor()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::HideActor);
	ES::HideActor& Data = CurStream->HideActorDataSet[CurIndexOfType];
	
	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data.TargetName);
	Target->AllRenderersActiveOff();
	return true;
}

bool UEventProcessor::ProcessShowActor()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::ShowActor);
	ES::ShowActor& Data = CurStream->ShowActorDataSet[CurIndexOfType];

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data.TargetName);
	Target->AllRenderersActiveOn();
	return true;
}

bool UEventProcessor::ProcessCameraFocus()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::CameraFocus);
	ES::CameraFocus& Data = CurStream->CameraFocusDataSet[CurIndexOfType];

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data.TargetName);
	ULevel* CurLevel = Target->GetWorld();
	CurLevel->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	return true;
}

bool UEventProcessor::ProcessDeactivatePlayerControl()
{
	DeactivatePlayerControl();
	return true;
}

// 편의 함수

std::string UEventProcessor::ToUpper(std::string_view _Name)
{
	return UEngineString::ToUpper(_Name);
}


// 시작 및 종료 함수

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
				IsPlayerActivated = false;
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

	IsRunningValue = false;
	CurIndexOfTypeMap.clear();
}


void UEventProcessor::RegisterStream(const UEventCondition& _Condition, UEventStream _Stream)
{
	AllStreams[_Condition] = _Stream;
}

void UEventProcessor::UnregisterStream(const UEventCondition& _Condition)
{
	AllStreams.erase(_Condition);
}
