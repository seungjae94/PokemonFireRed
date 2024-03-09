#include "EventProcessor.h"
#include <EngineBase/EngineDebug.h>
#include "EventStream.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "Player.h"
#include "DialogueWindow.h"
#include "MapNameCanvas.h"
#include "FadeCanvas.h"
#include "SoundManager.h"
#include "Trainer.h"

UEventProcessor::UEventProcessor()
{
}

UEventProcessor::~UEventProcessor()
{
}

void UEventProcessor::Tick(float _DeltaTime)
{
	while (true)
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
		case EEventType::SetActive:
			ProcessingResult = ProcessSetActive();
			break;
		case EEventType::Destroy:
			ProcessingResult = ProcessDestroy();
			break;
		case EEventType::Move:
			ProcessingResult = ProcessMove(_DeltaTime);
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
		case EEventType::PlayAnimation:
			ProcessingResult = ProcessPlayAnimation();
			break;
		case EEventType::PlayBgm:
			ProcessingResult = ProcessPlayBgm();
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
		case EEventType::WildBattle:
			ProcessingResult = ProcessWildBattle();
			break;
		case EEventType::TrainerBattle:
			ProcessingResult = ProcessTrainerBattle();
			break;
		case EEventType::Achieve:
			ProcessingResult = ProcessAchieve();
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
		else
		{
			return;
		}
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

bool UEventProcessor::ProcessSetActive()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::SetActive);
	ES::SetActive& Data = CurStream->SetActiveDataSet[CurIndexOfType];

	std::string MapName = ToUpper(Data.MapName);
	std::string TargetName = ToUpper(Data.TargetName);

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapName, TargetName);

	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "은 존재하지 않는 이벤트 타겟입니다. 존재하지 않는 이벤트 타겟을 켜고 끌 수 없습니다.");
		return true;
	}

	Target->SetActive(Data.Value);

	return true;
}

bool UEventProcessor::ProcessDestroy()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::Destroy);
	ES::Destroy& Data = CurStream->DestroyDataSet[CurIndexOfType];

	AActor* Actor = Data.Actor;

	if (nullptr == Actor)
	{
		MsgBoxAssert("이벤트 프로세서에서 nullptr인 액터를 삭제하려고 합니다.");
		return false;
	}

	Actor->Destroy();
	
	return true;
}

bool UEventProcessor::ProcessMove(float _DeltaTime)
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::Move);
	ES::Move& Data = CurStream->MoveDataSet[CurIndexOfType];
	
	// 이동 준비
	if (MoveState == EMoveState::None)
	{
		MoveState = EMoveState::Move;
		return SubprocessMoveStart();
	}

	return SubprocessMove(_DeltaTime);
}

bool UEventProcessor::SubprocessMoveStart()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::Move);
	ES::Move& Data = CurStream->MoveDataSet[CurIndexOfType];

	int TargetCount = static_cast<int>(Data.TargetNames.size());

	// 이동 관련 공유 데이터 초기화
	MoveTime = 1.0f / Data.MoveSpeed;
	MoveTimer = MoveTime;
	MovePathIndex = 0;
	MovePrevPoints.resize(TargetCount);
	MoveNextPoints.resize(TargetCount);
	MoveTargets.resize(TargetCount);
	MoveEnds.resize(TargetCount);

	std::string MapName = ToUpper(UEventManager::CurLevelName);
	for (int i = 0; i < TargetCount; ++i)
	{
		std::string TargetName = ToUpper(Data.TargetNames[i]);

		if (Data.Paths[i].size() == 0)
		{
			MsgBoxAssert(std::string(MapName) + ":" + TargetName + "의 강제 이동 경로 크기가 0입니다.");
			return false;
		}

		AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapName, TargetName);
		if (nullptr == Target)
		{
			MsgBoxAssert(std::string(MapName) + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
			return false;
		}

		// 이동 관련 타겟별 데이터 초기화
		Target->SetMoveState(ETargetMoveState::Walk);
		const std::vector<FTileVector>& Path = Data.Paths[i];
		if (Path[0] != FTileVector::Zero && Target->GetDirection() != Path[0])
		{
			// 제자리 걸음이 아닐 경우 다음 위치를 바라보도록 방향을 변경한다.
			Target->SetDirection(Path[0]);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		MovePrevPoints[i] = Target->GetPoint();
		MoveNextPoints[i] = MovePrevPoints[i] + Path[0];
		MoveTargets[i] = Target;
		MoveEnds[i] = false;
	}

	return false;
}

bool UEventProcessor::SubprocessMove(float _DeltaTime)
{
	MoveTimer -= _DeltaTime;

	int CurIndexOfType = GetCurIndexOfType(EEventType::Move);
	ES::Move& Data = CurStream->MoveDataSet[CurIndexOfType];
	std::string MapName = ToUpper(UEventManager::CurLevelName);

	bool IsAllMoveEnd = true;
	for (int i = 0; i < Data.Paths.size(); ++i)
	{
		// 이동이 끝난 타겟은 스킵한다.
		if (true == MoveEnds[i])
		{
			continue;
		}

		// 아직 이동중인 타겟이 있다.
		IsAllMoveEnd = false;

		const std::vector<FTileVector>& Path = Data.Paths[i];

		AEventTarget* Target = MoveTargets[i];
		std::string TargetName = ToUpper(Target->GetName());

		float t = MoveTimer / MoveTime;
		FVector TargetPos = UPokemonMath::Lerp(MoveNextPoints[i].ToFVector(), MovePrevPoints[i].ToFVector(), t);
		Target->SetActorLocation(TargetPos);

		if (true == Data.CameraFollow && TargetName == ToUpper(EN::Player))
		{
			Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
		}

		if (MoveTimer <= 0.0f)
		{
			UEventManager::SetPoint(MapName, Target->GetName(), FTileVector(Target->GetActorLocation()));

			// 이동이 끝난 타겟은 이동 종료 처리 및 이동 종료 마킹을 한다.
			if (MovePathIndex + 1 >= Path.size())
			{
				// 이동 종료 마킹
				MoveEnds[i] = true;

				// 이동 종료 처리
				Target->ResetFootOrder();
				Target->SetMoveState(ETargetMoveState::Idle);
				Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
				continue;
			}

			// 이동이 끝나지 않았기 때문에 다음 이동을 준비한다.
			MovePrevPoints[i] = Target->GetPoint();
			MoveNextPoints[i] = MovePrevPoints[i] + Path[MovePathIndex + 1];

			// 제자리 걸음이 아닐 경우 다음 위치를 바라보도록 방향을 변경한다.
			if (Path[MovePathIndex + 1] != FTileVector::Zero && Target->GetDirection() != Path[MovePathIndex + 1])
			{
				Target->SetDirection(Path[MovePathIndex + 1]);
			}
			Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		}
	}
	
	if (MoveTimer <= 0.0f)
	{
		MoveTimer = MoveTime;
		++MovePathIndex;
	}

	if (true == IsAllMoveEnd)
	{
		MoveState = EMoveState::None;
	}

	return IsAllMoveEnd;
}

bool UEventProcessor::ProcessMoveWithoutRestriction()
{
	float DeltaTime = UEventManager::GetDeltaTime();
	int CurIndexOfType = GetCurIndexOfType(EEventType::MoveWithoutRestriction);
	ES::MoveWithoutRestriction& Data = CurStream->MoveWithoutRestrictionDataSet[CurIndexOfType];

	std::string MapName = ToUpper(UEventManager::CurLevelName);
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
	AFadeCanvas* Canvas = UEventManager::FindCurLevelCommonCanvas<AFadeCanvas>(Global::FadeCanvas);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeIn);
	ES::FadeIn& Data = CurStream->FadeInDataSet[CurIndexOfType];

	switch (Data.FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeInBlack(Data.Time);
		break;
	case EFadeType::White:
		Canvas->FadeInWhite(Data.Time);
		break;
	case EFadeType::HCurtain:
		Canvas->OpenHCurtain(Data.Time);
		break;
	case EFadeType::VCurtain:
		Canvas->OpenVCurtain(Data.Time);
		break;
	default:
		MsgBoxAssert("아직 FadeIn 기능을 지원하지 않는 FadeType을 사용했습니다.");
		break;
	}

	return true;
}

bool UEventProcessor::ProcessFadeOut()
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	AFadeCanvas* Canvas = UEventManager::FindCurLevelCommonCanvas<AFadeCanvas>(Global::FadeCanvas);

	int CurIndexOfType = GetCurIndexOfType(EEventType::FadeOut);
	ES::FadeOut& Data = CurStream->FadeOutDataSet[CurIndexOfType];

	switch (Data.FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeOutBlack(Data.Time);
		break;
	case EFadeType::White:
		Canvas->FadeOutWhite(Data.Time);
		break;
	default:
		MsgBoxAssert("아직 FadeOut 기능을 지원하지 않는 FadeType을 사용했습니다.");
		break;
	}

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

bool UEventProcessor::ProcessPlayBgm()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::PlayBgm);
	ES::PlayBgm& Data = CurStream->PlayBgmDataSet[CurIndexOfType];
	USoundManager::PlayBgm(Data.Name);
	return true;
}

bool UEventProcessor::ProcessChat()
{
	ADialogueWindow* CurDialogueWindow = UEventManager::FindCurLevelDialogueWindow();

	int CurIndexOfType = GetCurIndexOfType(EEventType::Chat);
	ES::Chat& Data = CurStream->ChatDataSet[CurIndexOfType];

	// 아직 대화창을 켜지 않은 경우
	if (true == CurDialogueWindow->IsHide())
	{
		CurDialogueWindow->SetActive(true);
		CurDialogueWindow->Open(Data.Dialogue, Data.Color, Data.LineSpace);
		return false;
	}
	// 대화가 끝난 경우
	else if (true == CurDialogueWindow->IsEnd())
	{
		CurDialogueWindow->Hide();
		return true;
	}

	// 대화중인 경우
	return false;
}

bool UEventProcessor::ProcessShowMapName()
{
	AMapNameCanvas* MapNameCanvas = UEventManager::FindCurLevelCommonCanvas<AMapNameCanvas>(Global::MapNameWindow);

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

bool UEventProcessor::ProcessWildBattle()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::WildBattle);
	ES::WildBattle& Data = CurStream->WildBattleDataSet[CurIndexOfType];

	UEventManager::SaveEnemyEntry(Data.Entry);
	UEventManager::SetAsWildPokemonBattle();

	std::string PrevLevelName = UEventManager::CurLevelName;
	std::string NextLevelName = ToUpper(Global::BattleLevel);

	UEventManager::SetLevel(NextLevelName);
	if (false == IsPlayerActivated)
	{
		DeactivatePlayerControl();
	}

	return true;
}

bool UEventProcessor::ProcessTrainerBattle()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::TrainerBattle);
	ES::TrainerBattle& Data = CurStream->TrainerBattleDataSet[CurIndexOfType];

	UEventManager::SaveEnemyEntry(Data.Trainer->GetEntry());
	UEventManager::SetAsTrainerBattle(Data.Trainer);

	std::string PrevLevelName = UEventManager::CurLevelName;
	std::string NextLevelName = ToUpper(Global::BattleLevel);

	UEventManager::SetLevel(NextLevelName);
	if (false == IsPlayerActivated)
	{
		DeactivatePlayerControl();
	}

	return true;
}

bool UEventProcessor::ProcessAchieve()
{
	int CurIndexOfType = GetCurIndexOfType(EEventType::Achieve);
	ES::Achieve& Data = CurStream->AchieveDataSet[CurIndexOfType];
	UPlayerData::Achieve(Data.Achievement);
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
