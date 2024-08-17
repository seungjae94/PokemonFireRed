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

void UEventProcessor::Run(AEventTrigger* _Trigger, UEventStream* _Stream)
{
	Trigger = _Trigger;
	Stream = _Stream;
	IsRunningValue = true;
	CurCommandIndex = 0;

	if (true == Stream->DeactivatePlayer)
	{
		IsPlayerActivated = false;
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
		case EEventType::MoveDynamicPath:
			ProcessingResult = ProcessMoveDynamicPath(_DeltaTime);
			break;
		case EEventType::MoveWithoutRestriction:
			ProcessingResult = ProcessMoveWithoutRestriction();
			break;
		case EEventType::Surprise:
			ProcessingResult = ProcessSurprise();
			break;
		case EEventType::FadeIn:
			ProcessingResult = ProcessFadeIn();
			break;
		case EEventType::FadeOut:
			ProcessingResult = ProcessFadeOut();
			break;
		case EEventType::FadeInBgm:
			ProcessingResult = ProcessFadeInBgm();
			break;
		case EEventType::FadeOutBgm:
			ProcessingResult = ProcessFadeOutBgm();
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
		case EEventType::PlaySE:
			ProcessingResult = ProcessPlaySE();
			break;
		case EEventType::PauseBgm:
			ProcessingResult = ProcessPauseBgm();
			break;
		case EEventType::ResumeBgm:
			ProcessingResult = ProcessResumeBgm();
			break;
		case EEventType::Chat:
			ProcessingResult = ProcessChat();
			break;
		case EEventType::ShowMapName:
			ProcessingResult = ProcessShowMapName();
			break;
		case EEventType::ChangeArea:
			ProcessingResult = ProcessChangeArea();
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
		case EEventType::Unachieve:
			ProcessingResult = ProcessUnachieve();
			break;
		case EEventType::GainItem:
			ProcessingResult = ProcessGainItem();
			break;
		case EEventType::DeactivatePlayerControl:
			ProcessingResult = ProcessDeactivatePlayerControl();
			break;
		default:
			break;
		}

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

bool UEventProcessor::ProcessSetActive()
{
	ES::SetActive* Data = dynamic_cast<ES::SetActive*>(Stream->EventCommands[CurCommandIndex]);

	std::string MapName = ToUpper(Data->MapName);
	std::string TargetName = ToUpper(Data->TargetName);

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapName, TargetName);

	if (nullptr == Target)
	{
		MsgBoxAssert(MapName + ":" + TargetName + "은 존재하지 않는 이벤트 타겟입니다. 존재하지 않는 이벤트 타겟을 켜고 끌 수 없습니다.");
		return true;
	}

	Target->SetActive(Data->Value);

	return true;
}

bool UEventProcessor::ProcessDestroy()
{
	ES::Destroy* Data = dynamic_cast<ES::Destroy*>(Stream->EventCommands[CurCommandIndex]);

	AActor* Actor = Data->Actor;

	if (nullptr == Actor)
	{
		MsgBoxAssert("이벤트 프로세서에서 nullptr인 액터를 삭제하려고 합니다.");
		return false;
	}

	Actor->Destroy();

	AEventTarget* Target = dynamic_cast<AEventTarget*>(Actor);

	if (nullptr == Target)
	{
		return true;
	}

	// 이벤트 타겟일 경우
	std::string MapName = UEngineString::ToUpper(Target->GetWorld()->GetName());
	std::string TargetName = UEngineString::ToUpper(Target->GetName());
	UEventManager::RemoveTarget(MapName, TargetName);

	AEventTrigger* Trigger = dynamic_cast<AEventTrigger*>(Actor);

	if (nullptr == Trigger)
	{
		return true;
	}

	// 이벤트 트리거일 경우
	UEventManager::RemoveTrigger(MapName, TargetName);

	return true;
}

bool UEventProcessor::ProcessMove(float _DeltaTime)
{
	ES::Move* Data = dynamic_cast<ES::Move*>(Stream->EventCommands[CurCommandIndex]);

	// 이동 준비
	if (MoveState == EMoveState::None)
	{
		MoveState = EMoveState::Move;
		return SubprocessMoveStart(Data->TargetNames, Data->Paths, Data->MoveSpeed);
	}

	return SubprocessMove(Data->TargetNames, Data->Paths, Data->MoveSpeed, Data->CameraFollow, _DeltaTime);
}

bool UEventProcessor::ProcessMoveDynamicPath(float _DeltaTime)
{
	ES::MoveDynamicPath* Data = dynamic_cast<ES::MoveDynamicPath*>(Stream->EventCommands[CurCommandIndex]);

	// 이동 준비
	if (MoveState == EMoveState::None)
	{
		MoveState = EMoveState::Move;
		DynamicPaths.clear();

		std::vector<FTileVector> Path = Data->Generator();
		DynamicPaths.push_back(Path);

		// 경로의 크기가 0이면 이동할 필요가 없다.
		if (Path.size() == 0)
		{
			return true;
		}

		return SubprocessMoveStart({ Data->TargetName }, DynamicPaths, Data->MoveSpeed);
	}

	return SubprocessMove({ Data->TargetName }, DynamicPaths, Data->MoveSpeed, Data->CameraFollow, _DeltaTime);
}

bool UEventProcessor::SubprocessMoveStart(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed)
{
	int TargetCount = static_cast<int>(_TargetNames.size());

	// 이동 관련 공유 데이터 초기화
	MoveTime = 1.0f / _MoveSpeed;
	MoveTimer = MoveTime;
	MovePathIndex = 0;
	MovePrevPoints.resize(TargetCount);
	MoveNextPoints.resize(TargetCount);
	MoveTargets.resize(TargetCount);
	MoveEnds.resize(TargetCount);

	std::string MapName = ToUpper(UEventManager::CurLevelName);
	for (int i = 0; i < TargetCount; ++i)
	{
		std::string TargetName = ToUpper(_TargetNames[i]);

		if (_Paths[i].size() == 0)
		{
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
		const std::vector<FTileVector>& Path = _Paths[i];
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

bool UEventProcessor::SubprocessMove(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed, bool _CameraFollow, float _DeltaTime)
{
	MoveTimer -= _DeltaTime;

	std::string MapName = ToUpper(UEventManager::CurLevelName);

	bool IsAllMoveEnd = true;
	for (int i = 0; i < _Paths.size(); ++i)
	{
		// 이동이 끝난 타겟은 스킵한다.
		if (true == MoveEnds[i])
		{
			continue;
		}

		// 아직 이동중인 타겟이 있다.
		IsAllMoveEnd = false;

		const std::vector<FTileVector>& Path = _Paths[i];

		AEventTarget* Target = MoveTargets[i];
		std::string TargetName = ToUpper(Target->GetName());

		float t = MoveTimer / MoveTime;
		FVector TargetPos = UPokemonMath::Lerp(MoveNextPoints[i].ToFVector(), MovePrevPoints[i].ToFVector(), t);
		Target->SetActorLocation(TargetPos);

		if (true == _CameraFollow && TargetName == ToUpper(EN::PlayerCharacter))
		{
			Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
		}

		if (MoveTimer <= 0.0f)
		{
			UEventManager::SetPoint(MapName, Target->GetName(), MoveNextPoints[i]);

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
	ES::MoveWithoutRestriction* Data = dynamic_cast<ES::MoveWithoutRestriction*>(Stream->EventCommands[CurCommandIndex]);

	std::string MapName = ToUpper(UEventManager::CurLevelName);
	std::string TargetName = ToUpper(Data->TargetName);

	if (Data->Path.size() <= 0)
	{
		// 이동할 필요가 없다.
		return true;
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
	else if (MoveWRPathIndex + 1 >= Data->Path.size())
	{
		// 이동 종료
		PostProcessMoveWR(Target);
		return true;
	}
	else
	{
		float PathSize = Data->Path[MoveWRPathIndex + 1].Size2D() / Global::FloatTileSize;
		MoveWRTime = PathSize / Data->MoveSpeed;

		MoveWRPrevPos = Target->GetActorLocation();
		MoveWRNextPos = MoveWRPrevPos + Data->Path[MoveWRPathIndex + 1];
		MoveWRTimer = MoveWRTime;

		// 제자리 걸음이 아닐 경우 Path에 입력된 벡터를 가장 가까운 좌표축에 투영하고 정규화한 벡터로 타겟의 방향을 바꾼다.
		FTileVector ProjectedDirection = UPokemonMath::ProjectToTileVector(Data->Path[MoveWRPathIndex + 1]);
		if (ProjectedDirection != FTileVector::Zero && Target->GetDirection() != ProjectedDirection)
		{
			Target->SetDirection(ProjectedDirection);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		MoveWRPathIndex++;
	}
	return false;
}

bool UEventProcessor::ProcessSurprise()
{
	ES::Surprise* Data = dynamic_cast<ES::Surprise*>(Stream->EventCommands[CurCommandIndex]);

	if (true == SurpriseFirstTick)
	{
		AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data->TargetName);
		Surprise = UEventManager::FindCurLevelTarget<ASurprise>(EN::Surprise);
		UEventManager::SetPoint(UEventManager::CurLevelName, EN::Surprise, Target->GetPoint() + FTileVector(0, -2));
		Surprise->Play();
		Surprise->SetActive(true);
		SurpriseFirstTick = false;
		return false;
	}
	else if (true == Surprise->IsEnd())
	{
		Surprise->SetActive(false);
		SurpriseFirstTick = true;
		return true;
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

	ES::FadeIn* Data = dynamic_cast<ES::FadeIn*>(Stream->EventCommands[CurCommandIndex]);

	switch (Data->FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeInBlack(Data->Time);
		break;
	case EFadeType::White:
		Canvas->FadeInWhite(Data->Time);
		break;
	case EFadeType::HCurtain:
		Canvas->OpenHCurtain(Data->Time);
		break;
	case EFadeType::VCurtain:
		Canvas->OpenVCurtain(Data->Time);
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

	ES::FadeOut* Data = dynamic_cast<ES::FadeOut*>(Stream->EventCommands[CurCommandIndex]);

	switch (Data->FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeOutBlack(Data->Time);
		break;
	case EFadeType::White:
		Canvas->FadeOutWhite(Data->Time);
		break;
	default:
		MsgBoxAssert("아직 FadeOut 기능을 지원하지 않는 FadeType을 사용했습니다.");
		break;
	}

	return true;
}

bool UEventProcessor::ProcessFadeInBgm()
{
	ES::FadeInBgm* Data = dynamic_cast<ES::FadeInBgm*>(Stream->EventCommands[CurCommandIndex]);
	USoundManager::FadeBgm(Data->TargetVolume, Data->Time);
	return true;
}

bool UEventProcessor::ProcessFadeOutBgm()
{
	ES::FadeOutBgm* Data = dynamic_cast<ES::FadeOutBgm*>(Stream->EventCommands[CurCommandIndex]);
	USoundManager::FadeBgm(0.0f, Data->Time);
	return true;
}

bool UEventProcessor::ProcessWait()
{
	static bool IsBegin = true;
	static float Timer = 0.0f;

	float DeltaTime = UEventManager::GetDeltaTime();

	ES::Wait* Data = dynamic_cast<ES::Wait*>(Stream->EventCommands[CurCommandIndex]);

	if (true == IsBegin)
	{
		Timer = Data->Time;
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
	ES::PlayAnimation* Data = dynamic_cast<ES::PlayAnimation*>(Stream->EventCommands[CurCommandIndex]);

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data->TargetName);

	if (true == PlayAnimIsFirstTick)
	{
		// 애니메이션 재생
		switch (Data->AnimTarget)
		{
		case EAnimTarget::UpperBodyOnly:
			Target->UpperBodyRenderer->ChangeAnimation(Data->AnimName + Global::SuffixUpperBody);
			break;
		case EAnimTarget::LowerBodyOnly:
			Target->LowerBodyRenderer->ChangeAnimation(Data->AnimName + Global::SuffixLowerBody);
			break;
		case EAnimTarget::All:
			if (1 == Target->Height)
			{
				Target->LowerBodyRenderer->ChangeAnimation(Data->AnimName);
			}
			else
			{
				Target->UpperBodyRenderer->ChangeAnimation(Data->AnimName + Global::SuffixUpperBody);
				Target->LowerBodyRenderer->ChangeAnimation(Data->AnimName + Global::SuffixLowerBody);
			}
			break;
		default:
			break;
		}

		if (false == Data->Wait)
		{
			// 비동기적 재생인 경우
			return true;
		}

		PlayAnimIsFirstTick = false;
		return false;
	}

	bool IsAnimEnd = false;

	switch (Data->AnimTarget)
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
	ES::PlayBgm* Data = dynamic_cast<ES::PlayBgm*>(Stream->EventCommands[CurCommandIndex]);
	USoundManager::PlayBgm(Data->Name);
	return true;
}

bool UEventProcessor::ProcessPlaySE()
{
	ES::PlaySE* Data = dynamic_cast<ES::PlaySE*>(Stream->EventCommands[CurCommandIndex]);

	USoundManager::PlaySE(Data->Name, Data->MuteTime);
	return true;
}

bool UEventProcessor::ProcessChat()
{
	ADialogueWindow* CurDialogueWindow = UEventManager::FindCurLevelDialogueWindow();

	ES::Chat* Data = dynamic_cast<ES::Chat*>(Stream->EventCommands[CurCommandIndex]);

	// 아직 대화창을 켜지 않은 경우
	if (true == CurDialogueWindow->IsHide())
	{
		CurDialogueWindow->SetActive(true);
		CurDialogueWindow->Open(Data->Dialogue, Data->Color, Data->LineSpace);
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

bool UEventProcessor::ProcessPauseBgm()
{
	USoundManager::PauseBgm();
	return true;
}

bool UEventProcessor::ProcessResumeBgm()
{
	USoundManager::ResumeBgm();
	return true;
}

bool UEventProcessor::ProcessShowMapName()
{
	AMapNameCanvas* MapNameCanvas = UEventManager::FindCurLevelCommonCanvas<AMapNameCanvas>(Global::MapNameWindow);

	ES::ShowMapName* Data = dynamic_cast<ES::ShowMapName*>(Stream->EventCommands[CurCommandIndex]);
	MapNameCanvas->SetActive(true);
	MapNameCanvas->AllRenderersActiveOn();
	MapNameCanvas->Open(Data->MapName);
	return true;
}

bool UEventProcessor::ProcessChangeArea()
{
	ES::ChangeArea* Data = dynamic_cast<ES::ChangeArea*>(Stream->EventCommands[CurCommandIndex]);
	UEventManager::ChangeArea(Data->AreaName, Data->AreaBgm);
	return true;
}

bool UEventProcessor::ProcessChangeLevel()
{
	ES::ChangeLevel* Data = dynamic_cast<ES::ChangeLevel*>(Stream->EventCommands[CurCommandIndex]);

	std::string PrevLevelName = UEventManager::CurLevelName;
	std::string NextLevelName = ToUpper(Data->LevelName);

	UEventManager::SetLevel(NextLevelName);
	if (false == IsPlayerActivated)
	{
		DeactivatePlayerControl();
	}

	return true;
}

bool UEventProcessor::ProcessChangePoint()
{
	ES::ChangePoint* Data = dynamic_cast<ES::ChangePoint*>(Stream->EventCommands[CurCommandIndex]);
	UEventManager::SetPoint(Data->LevelName, Data->TargetName, Data->Point);
	return true;
}

bool UEventProcessor::ProcessChangePosition()
{
	ES::ChangePosition* Data = dynamic_cast<ES::ChangePosition*>(Stream->EventCommands[CurCommandIndex]);

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(Data->LevelName, Data->TargetName);
	Target->SetActorLocation(Data->Position);
	return true;
}

bool UEventProcessor::ProcessChangeDirection()
{
	ES::ChangeDirection* Data = dynamic_cast<ES::ChangeDirection*>(Stream->EventCommands[CurCommandIndex]);
	UEventManager::SetDirection(Data->LevelName, Data->TargetName, Data->Direction);
	return true;
}

bool UEventProcessor::ProcessStarePlayer()
{
	ES::StarePlayer* Data = dynamic_cast<ES::StarePlayer*>(Stream->EventCommands[CurCommandIndex]);

	std::string CurLevelName = UEventManager::GetCurLevelName();
	APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	UEventManager::SetDirection(CurLevelName, Data->TargetName, -Player->Direction);
	return true;
}

bool UEventProcessor::ProcessHideActor()
{
	ES::HideActor* Data = dynamic_cast<ES::HideActor*>(Stream->EventCommands[CurCommandIndex]);

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data->TargetName);
	Target->AllRenderersActiveOff();
	return true;
}

bool UEventProcessor::ProcessShowActor()
{
	ES::ShowActor* Data = dynamic_cast<ES::ShowActor*>(Stream->EventCommands[CurCommandIndex]);

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data->TargetName);
	Target->AllRenderersActiveOn();
	return true;
}

bool UEventProcessor::ProcessCameraFocus()
{
	ES::CameraFocus* Data = dynamic_cast<ES::CameraFocus*>(Stream->EventCommands[CurCommandIndex]);

	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(Data->TargetName);
	ULevel* CurLevel = Target->GetWorld();
	CurLevel->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	return true;
}

bool UEventProcessor::ProcessWildBattle()
{
	ES::WildBattle* Data = dynamic_cast<ES::WildBattle*>(Stream->EventCommands[CurCommandIndex]);

	UEventManager::SaveEnemyEntry(Data->Entry);
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
	ES::TrainerBattle* Data = dynamic_cast<ES::TrainerBattle*>(Stream->EventCommands[CurCommandIndex]);

	UEventManager::SaveEnemyEntry(Data->Trainer->GetEntry());
	UEventManager::SetAsTrainerBattle(Data->Trainer);

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
	ES::Achieve* Data = dynamic_cast<ES::Achieve*>(Stream->EventCommands[CurCommandIndex]);
	UPlayerData::Achieve(Data->Achievement);
	return true;
}

bool UEventProcessor::ProcessUnachieve()
{
	ES::Unachieve* Data = dynamic_cast<ES::Unachieve*>(Stream->EventCommands[CurCommandIndex]);
	UPlayerData::Unachieve(Data->Achievement);
	return true;
}

bool UEventProcessor::ProcessGainItem()
{
	ES::GainItem* Data = dynamic_cast<ES::GainItem*>(Stream->EventCommands[CurCommandIndex]);
	UPlayerData::GainItem(Data->ItemId, Data->Count);
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
