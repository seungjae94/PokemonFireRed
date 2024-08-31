#include "EventCommand.h"
#include <EngineBase/EngineString.h>
#include "EventCommand_ProcessingData.h"
#include "EventManager.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "Surprise.h"
#include "FadeCanvas.h"
#include "SoundManager.h"
#include "DialogueWindow.h"
#include "PlayerCharacter.h"
#include "Trainer.h"
#include "MapNameCanvas.h"

#define EP UEventProcessor
#define EM UEventManager

bool ES::SetActive::Process(float _DeltaTime)
{
	std::string MapNameUpper = UEngineString::ToUpper(MapName);
	std::string TargetNameUpper = UEngineString::ToUpper(TargetName);

	AEventTarget* Target = EM::FindTarget<AEventTarget>(MapNameUpper, TargetNameUpper);

	if (nullptr == Target)
	{
		MsgBoxAssert(MapNameUpper + ":" + TargetNameUpper + "�� �������� �ʴ� �̺�Ʈ Ÿ���Դϴ�. �������� �ʴ� �̺�Ʈ Ÿ���� �Ѱ� �� �� �����ϴ�.");
		return true;
	}

	Target->SetActive(Value);

	return true;
}

bool ES::Destroy::Process(float _DeltaTime)
{
	if (nullptr == Actor)
	{
		MsgBoxAssert("�̺�Ʈ ���μ������� nullptr�� ���͸� �����Ϸ��� �մϴ�.");
		return false;
	}

	Actor->Destroy();

	AEventTarget* Target = dynamic_cast<AEventTarget*>(Actor);

	if (nullptr == Target)
	{
		return true;
	}

	// �̺�Ʈ Ÿ���� ���
	std::string MapName = UEngineString::ToUpper(Target->GetWorld()->GetName());
	std::string TargetName = UEngineString::ToUpper(Target->GetName());
	EM::RemoveTarget(MapName, TargetName);

	AEventTrigger* Trigger = dynamic_cast<AEventTrigger*>(Actor);

	if (nullptr == Trigger)
	{
		return true;
	}

	// �̺�Ʈ Ʈ������ ���
	EM::RemoveTrigger(MapName, TargetName);

	return true;
}

bool ES::MoveBase::MoveStart(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed)
{
	int TargetCount = static_cast<int>(_TargetNames.size());

	// �̵� ���� ���� ������ �ʱ�ȭ
	EP::MoveBaseData.MoveTime = 1.0f / _MoveSpeed;
	EP::MoveBaseData.MoveTimer = EP::MoveBaseData.MoveTime;
	EP::MoveBaseData.MovePathIndex = 0;
	EP::MoveBaseData.MovePrevPoints.resize(TargetCount);
	EP::MoveBaseData.MoveNextPoints.resize(TargetCount);
	EP::MoveBaseData.MoveTargets.resize(TargetCount);
	EP::MoveBaseData.MoveEnds.resize(TargetCount);

	std::string MapName = UEngineString::ToUpper(EM::GetCurLevelName());
	for (int i = 0; i < TargetCount; ++i)
	{
		std::string TargetName = UEngineString::ToUpper(_TargetNames[i]);

		if (_Paths[i].size() == 0)
		{
			return false;
		}

		AEventTarget* Target = EM::FindTarget<AEventTarget>(MapName, TargetName);
		if (nullptr == Target)
		{
			MsgBoxAssert(std::string(MapName) + ":" + TargetName + "�� �������� �ʴ� �̺�Ʈ Ÿ���Դϴ�.�������� �ʴ� �̺�Ʈ Ÿ���� �̵���Ű���� �߽��ϴ�.");
			return false;
		}

		// �̵� ���� Ÿ�ٺ� ������ �ʱ�ȭ
		Target->SetMoveState(ETargetMoveState::Walk);
		const std::vector<FTileVector>& Path = _Paths[i];
		if (Path[0] != FTileVector::Zero && Target->GetDirection() != Path[0])
		{
			// ���ڸ� ������ �ƴ� ��� ���� ��ġ�� �ٶ󺸵��� ������ �����Ѵ�.
			Target->SetDirection(Path[0]);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		EP::MoveBaseData.MovePrevPoints[i] = Target->GetPoint();
		EP::MoveBaseData.MoveNextPoints[i] = EP::MoveBaseData.MovePrevPoints[i] + Path[0];
		EP::MoveBaseData.MoveTargets[i] = Target;
		EP::MoveBaseData.MoveEnds[i] = false;
	}

	return false;
}

bool ES::MoveBase::MoveUpdate(const std::vector<std::string>& _TargetNames, const std::vector<std::vector<FTileVector>>& _Paths, float _MoveSpeed, bool _CameraFollow, float _DeltaTime)
{
	EP::MoveBaseData.MoveTimer -= _DeltaTime;

	std::string MapName = UEngineString::ToUpper(EM::GetCurLevelName());

	bool IsAllMoveEnd = true;
	for (int i = 0; i < _Paths.size(); ++i)
	{
		// �̵��� ���� Ÿ���� ��ŵ�Ѵ�.
		if (true == EP::MoveBaseData.MoveEnds[i])
		{
			continue;
		}

		// ���� �̵����� Ÿ���� �ִ�.
		IsAllMoveEnd = false;

		const std::vector<FTileVector>& Path = _Paths[i];

		AEventTarget* Target = EP::MoveBaseData.MoveTargets[i];
		std::string TargetName = UEngineString::ToUpper(Target->GetName());

		float t = EP::MoveBaseData.MoveTimer / EP::MoveBaseData.MoveTime;
		FVector TargetPos = UPokemonMath::Lerp(EP::MoveBaseData.MoveNextPoints[i].ToFVector(), EP::MoveBaseData.MovePrevPoints[i].ToFVector(), t);
		Target->SetActorLocation(TargetPos);

		if (true == _CameraFollow && TargetName == UEngineString::ToUpper(EN::PlayerCharacter))
		{
			Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
		}

		if (EP::MoveBaseData.MoveTimer <= 0.0f)
		{
			EM::SetPoint(MapName, Target->GetName(), EP::MoveBaseData.MoveNextPoints[i]);

			// �̵��� ���� Ÿ���� �̵� ���� ó�� �� �̵� ���� ��ŷ�� �Ѵ�.
			if (EP::MoveBaseData.MovePathIndex + 1 >= Path.size())
			{
				// �̵� ���� ��ŷ
				EP::MoveBaseData.MoveEnds[i] = true;

				// �̵� ���� ó��
				Target->ResetFootOrder();
				Target->SetMoveState(ETargetMoveState::Idle);
				Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
				continue;
			}

			// �̵��� ������ �ʾұ� ������ ���� �̵��� �غ��Ѵ�.
			EP::MoveBaseData.MovePrevPoints[i] = Target->GetPoint();
			EP::MoveBaseData.MoveNextPoints[i] = EP::MoveBaseData.MovePrevPoints[i] + Path[EP::MoveBaseData.MovePathIndex + 1];

			// ���ڸ� ������ �ƴ� ��� ���� ��ġ�� �ٶ󺸵��� ������ �����Ѵ�.
			if (Path[EP::MoveBaseData.MovePathIndex + 1] != FTileVector::Zero 
				&& Target->GetDirection() != Path[EP::MoveBaseData.MovePathIndex + 1])
			{
				Target->SetDirection(Path[EP::MoveBaseData.MovePathIndex + 1]);
			}
			Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		}
	}

	if (EP::MoveBaseData.MoveTimer <= 0.0f)
	{
		EP::MoveBaseData.MoveTimer = EP::MoveBaseData.MoveTime;
		++EP::MoveBaseData.MovePathIndex;
	}

	if (true == IsAllMoveEnd)
	{
		EP::MoveBaseData.bIsMoving = false;
	}

	return IsAllMoveEnd;
}


ES::Move::Move(std::string_view _TargetName, const std::vector<FTileVector>& _Path, float _MoveSpeed, bool _CameraFollow)
	: MoveBase(EEventType::Move, _MoveSpeed, _CameraFollow)
{
	TargetNames.push_back(std::string(_TargetName));
	Paths.push_back(_Path);
}

bool ES::Move::Process(float _DeltaTime)
{
	// �̵� �غ�
	if (EP::MoveBaseData.bIsMoving == false)
	{
		EP::MoveBaseData.bIsMoving = true;
		return MoveStart(TargetNames, Paths, MoveSpeed);
	}

	return MoveUpdate(TargetNames, Paths, MoveSpeed, CameraFollow, _DeltaTime);
}

bool ES::MoveDynamicPath::Process(float _DeltaTime)
{
	// �̵� �غ�
	if (EP::MoveBaseData.bIsMoving == false)
	{
		EP::MoveBaseData.bIsMoving = true;
		EP::MoveDynamicPathData.DynamicPaths.clear();

		std::vector<FTileVector> Path = Generator();
		EP::MoveDynamicPathData.DynamicPaths.push_back(Path);

		// ����� ũ�Ⱑ 0�̸� �̵��� �ʿ䰡 ����.
		if (Path.size() == 0)
		{
			return true;
		}

		return MoveStart({ TargetName }, EP::MoveDynamicPathData.DynamicPaths, MoveSpeed);
	}

	return MoveUpdate({ TargetName }, EP::MoveDynamicPathData.DynamicPaths, MoveSpeed, CameraFollow, _DeltaTime);
}

bool ES::MoveWithoutRestriction::Process(float _DeltaTime)
{
	float DeltaTime = UEventManager::GetDeltaTime();

	std::string MapNameUpper = UEngineString::ToUpper(UEventManager::GetCurLevelName());
	std::string TargetNameUpper = UEngineString::ToUpper(TargetName);

	if (Path.size() <= 0)
	{
		// �̵��� �ʿ䰡 ����.
		return true;
	}

	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(MapNameUpper, TargetNameUpper);
	if (nullptr == Target)
	{
		MsgBoxAssert(MapNameUpper + ":" + TargetNameUpper + "�� �������� �ʴ� �̺�Ʈ Ÿ���Դϴ�.�������� �ʴ� �̺�Ʈ Ÿ���� �̵���Ű���� �߽��ϴ�.");
		return false;
	}

	// �̵� �̺�Ʈ ����
	if (EP::MoveWRData.MoveWRPathIndex == -1)
	{
		EP::MoveWRData.MoveWRTimer = -1.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
	}

	if (EP::MoveWRData.MoveWRTimer > 0.0f)
	{
		EP::MoveWRData.MoveWRTimer -= DeltaTime;

		float t = (EP::MoveWRData.MoveWRTime - EP::MoveWRData.MoveWRTimer) / EP::MoveWRData.MoveWRTime;
		FVector TargetPos = UPokemonMath::Lerp(EP::MoveWRData.MoveWRPrevPos, EP::MoveWRData.MoveWRNextPos, t);
		Target->SetActorLocation(TargetPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	}
	else if (EP::MoveWRData.MoveWRPathIndex + 1 >= Path.size())
	{
		// �̵� ����
		ProcessEnd(Target);
		return true;
	}
	else
	{
		float PathSize = Path[EP::MoveWRData.MoveWRPathIndex + 1].Size2D() / Global::FloatTileSize;
		EP::MoveWRData.MoveWRTime = PathSize / MoveSpeed;

		EP::MoveWRData.MoveWRPrevPos = Target->GetActorLocation();
		EP::MoveWRData.MoveWRNextPos = EP::MoveWRData.MoveWRPrevPos + Path[EP::MoveWRData.MoveWRPathIndex + 1];
		EP::MoveWRData.MoveWRTimer = EP::MoveWRData.MoveWRTime;

		// ���ڸ� ������ �ƴ� ��� Path�� �Էµ� ���͸� ���� ����� ��ǥ�࿡ �����ϰ� ����ȭ�� ���ͷ� Ÿ���� ������ �ٲ۴�.
		FTileVector ProjectedDirection = UPokemonMath::ProjectToTileVector(Path[EP::MoveWRData.MoveWRPathIndex + 1]);
		if (ProjectedDirection != FTileVector::Zero && Target->GetDirection() != ProjectedDirection)
		{
			Target->SetDirection(ProjectedDirection);
		}
		Target->ChangeMoveAnimation(Target->GetMoveState(), Target->GetDirection());
		EP::MoveWRData.MoveWRPathIndex++;
	}
	return false;
}

void ES::MoveWithoutRestriction::ProcessEnd(AEventTarget* _Target)
{
	EP::MoveWRData.MoveWRPrevPos = FVector::Zero;
	EP::MoveWRData.MoveWRNextPos = FVector::Zero;
	EP::MoveWRData.MoveWRPathIndex = -1;		// -1�� ù ��° ƽ���� �ǹ��Ѵ�.
	EP::MoveWRData.MoveWRTime = 0.0f;
	EP::MoveWRData.MoveWRTimer = 0.0f;
	_Target->ResetFootOrder();
	_Target->SetMoveState(ETargetMoveState::Idle);
	_Target->ChangeMoveAnimation(_Target->GetMoveState(), _Target->GetDirection());
}

bool ES::Surprise::Process(float _DeltaTime)
{
	if (true == EP::SurpriseData.SurpriseFirstTick)
	{
		AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(TargetName);
		EP::SurpriseData.SurpriseIcon = UEventManager::FindCurLevelTarget<ASurprise>(EN::SurpriseIcon);
		UEventManager::SetPoint(UEventManager::GetCurLevelName(), EN::SurpriseIcon, Target->GetPoint() + FTileVector(0, -2));
		EP::SurpriseData.SurpriseIcon->Play();
		EP::SurpriseData.SurpriseIcon->SetActive(true);
		EP::SurpriseData.SurpriseFirstTick = false;
		return false;
	}
	else if (true == EP::SurpriseData.SurpriseIcon->IsEnd())
	{
		EP::SurpriseData.SurpriseIcon->SetActive(false);
		EP::SurpriseData.SurpriseFirstTick = true;
		return true;
	}

	return false;
}

bool ES::FadeIn::Process(float _DeltaTime)
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	AFadeCanvas* Canvas = UEventManager::FindCurLevelCommonCanvas<AFadeCanvas>(Global::FadeCanvas);

	switch (FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeInBlack(Time);
		break;
	case EFadeType::White:
		Canvas->FadeInWhite(Time);
		break;
	case EFadeType::HCurtain:
		Canvas->OpenHCurtain(Time);
		break;
	case EFadeType::VCurtain:
		Canvas->OpenVCurtain(Time);
		break;
	default:
		MsgBoxAssert("���� FadeIn ����� �������� �ʴ� FadeType�� ����߽��ϴ�.");
		break;
	}

	return true;
}

bool ES::FadeOut::Process(float _DeltaTime)
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	AFadeCanvas* Canvas = UEventManager::FindCurLevelCommonCanvas<AFadeCanvas>(Global::FadeCanvas);

	switch (FadeType)
	{
	case EFadeType::Black:
		Canvas->FadeOutBlack(Time);
		break;
	case EFadeType::White:
		Canvas->FadeOutWhite(Time);
		break;
	default:
		MsgBoxAssert("���� FadeOut ����� �������� �ʴ� FadeType�� ����߽��ϴ�.");
		break;
	}

	return true;
}

bool ES::FadeInBgm::Process(float _DeltaTime)
{
	USoundManager::FadeBgm(TargetVolume, Time);
	return true;
}

bool ES::FadeOutBgm::Process(float _DeltaTime)
{
	USoundManager::FadeBgm(0.0f, Time);
	return true;
}

bool ES::Wait::Process(float _DeltaTime)
{
	float DeltaTime = UEventManager::GetDeltaTime();

	if (true == EP::WaitData.IsBegin)
	{
		EP::WaitData.Timer = Time;
		EP::WaitData.IsBegin = false;
	}

	if (EP::WaitData.Timer <= 0.0f)
	{
		EP::WaitData.IsBegin = true;
		return true;
	}

	EP::WaitData.Timer -= DeltaTime;

	return false;
}

bool ES::PlayAnimation::Process(float _DeltaTime)
{
	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(TargetName);

	if (true == EP::IsPlayerAnimFirstTick())
	{
		// �ִϸ��̼� ���
		switch (AnimTarget)
		{
		case EAnimTarget::UpperBodyOnly:
			Target->GetUpperBodyRenderer()->ChangeAnimation(AnimName + Global::SuffixUpperBody);
			break;
		case EAnimTarget::LowerBodyOnly:
			Target->GetLowerBodyRenderer()->ChangeAnimation(AnimName + Global::SuffixLowerBody);
			break;
		case EAnimTarget::All:
			if (1 == Target->GetHeight())
			{
				Target->GetLowerBodyRenderer()->ChangeAnimation(AnimName);
			}
			else
			{
				Target->GetUpperBodyRenderer()->ChangeAnimation(AnimName + Global::SuffixUpperBody);
				Target->GetLowerBodyRenderer()->ChangeAnimation(AnimName + Global::SuffixLowerBody);
			}
			break;
		default:
			break;
		}

		if (false == Wait)
		{
			// �񵿱��� ����� ���
			return true;
		}

		EP::SetIsPlayerAnimFirstTick(false);
		return false;
	}

	bool IsAnimEnd = false;

	switch (AnimTarget)
	{
	case EAnimTarget::UpperBodyOnly:
		IsAnimEnd = Target->GetUpperBodyRenderer()->IsCurAnimationEnd();
		break;
	case EAnimTarget::LowerBodyOnly:
		IsAnimEnd = Target->GetLowerBodyRenderer()->IsCurAnimationEnd();
		break;
	case EAnimTarget::All:
		if (1 == Target->GetHeight())
		{
			IsAnimEnd = Target->GetLowerBodyRenderer()->IsCurAnimationEnd();
		}
		else
		{
			IsAnimEnd = Target->GetUpperBodyRenderer()->IsCurAnimationEnd() && Target->GetLowerBodyRenderer()->IsCurAnimationEnd();
		}
		break;
	default:
		break;
	}

	if (true == IsAnimEnd)
	{
		// �Լ� ���� �ʱ�ȭ �� �̺�Ʈ ����
		EP::SetIsPlayerAnimFirstTick(true);
		return true;
	}

	return false;
}

bool ES::PlayBgm::Process(float _DeltaTime)
{
	USoundManager::PlayBgm(Name);
	return true;
}

bool ES::PlaySE::Process(float _DeltaTime)
{
	USoundManager::PlaySE(Name, MuteTime);
	return true;
}

bool ES::PauseBgm::Process(float _DeltaTime)
{
	USoundManager::PauseBgm();
	return true;
}

bool ES::ResumeBgm::Process(float _DeltaTime)
{
	USoundManager::ResumeBgm();
	return true;
}

bool ES::Chat::Process(float _DeltaTime)
{
	ADialogueWindow* CurDialogueWindow = UEventManager::FindCurLevelDialogueWindow();

	// ���� ��ȭâ�� ���� ���� ���
	if (true == CurDialogueWindow->IsHide())
	{
		CurDialogueWindow->SetActive(true);
		CurDialogueWindow->Open(Dialogue, Color, LineSpace);
		return false;
	}
	// ��ȭ�� ���� ���
	else if (true == CurDialogueWindow->IsEnd())
	{
		CurDialogueWindow->Hide();
		return true;
	}

	// ��ȭ���� ���
	return false;
}

bool ES::ShowMapName::Process(float _DeltaTime)
{
	AMapNameCanvas* MapNameCanvas = UEventManager::FindCurLevelCommonCanvas<AMapNameCanvas>(Global::MapNameWindow);

	MapNameCanvas->SetActive(true);
	MapNameCanvas->AllRenderersActiveOn();
	MapNameCanvas->Open(MapName);
	return true;
}

bool ES::ChangeArea::Process(float _DeltaTime)
{
	UEventManager::ChangeArea(AreaName, AreaBgm);
	return true;
}

bool ES::ChangeLevel::Process(float _DeltaTime)
{
	std::string PrevLevelName = UEventManager::GetCurLevelName();
	std::string NextLevelName = UEngineString::ToUpper(LevelName);

	UEventManager::SetLevel(NextLevelName);
	if (false == EP::IsPlayerActivated())
	{
		EP::DeactivatePlayerControl();
	}

	return true;
}

bool ES::ChangePoint::Process(float _DeltaTime)
{
	UEventManager::SetPoint(LevelName, TargetName, Point);
	return true;
}

bool ES::ChangePosition::Process(float _DeltaTime)
{
	AEventTarget* Target = UEventManager::FindTarget<AEventTarget>(LevelName, TargetName);
	Target->SetActorLocation(Position);
	return true;
}

bool ES::ChangeDirection::Process(float _DeltaTime)
{
	UEventManager::SetDirection(LevelName, TargetName, Direction);
	return true;
}

bool ES::StarePlayer::Process(float _DeltaTime)
{
	std::string CurLevelName = UEventManager::GetCurLevelName();
	APlayerCharacter* Player = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	UEventManager::SetDirection(CurLevelName, TargetName, -Player->GetDirection());
	return true;
}

bool ES::HideActor::Process(float _DeltaTime)
{
	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(TargetName);
	Target->AllRenderersActiveOff();
	return true;
}

bool ES::ShowActor::Process(float _DeltaTime)
{
	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(TargetName);
	Target->AllRenderersActiveOn();
	return true;
}

bool ES::CameraFocus::Process(float _DeltaTime)
{
	AEventTarget* Target = UEventManager::FindCurLevelTarget<AEventTarget>(TargetName);
	ULevel* CurLevel = Target->GetWorld();
	CurLevel->SetCameraPos(Target->GetActorLocation() - Global::HalfScreen);
	return true;
}

bool ES::DeactivatePlayerControl::Process(float _DeltaTime)
{
	APlayerCharacter* CurPlayer = UEventManager::FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);

	// �� ������ �ƴ� ��� �÷��̾��� ���¸� �ٲٷ��� ���� �ʴ´�.
	if (nullptr == CurPlayer)
	{
		return true;
	}

	CurPlayer->StateChange(EPlayerState::OutOfControl);

	return true;
}

bool ES::WildBattle::Process(float _DeltaTime)
{
	UEventManager::SaveEnemyEntry(Entry);
	UEventManager::SetAsWildPokemonBattle();

	std::string PrevLevelName = UEventManager::GetCurLevelName();
	std::string NextLevelName = UEngineString::ToUpper(Global::BattleLevel);

	UEventManager::SetLevel(NextLevelName);
	if (false == EP::IsPlayerActivated())
	{
		EP::DeactivatePlayerControl();
	}

	return true;
}

bool ES::TrainerBattle::Process(float _DeltaTime)
{
	UEventManager::SaveEnemyEntry(Trainer->GetEntry());
	UEventManager::SetAsTrainerBattle(Trainer);

	std::string PrevLevelName = UEventManager::GetCurLevelName();
	std::string NextLevelName = UEngineString::ToUpper(Global::BattleLevel);

	UEventManager::SetLevel(NextLevelName);
	if (false == EP::IsPlayerActivated())
	{
		EP::DeactivatePlayerControl();
	}

	return true;
}

bool ES::Achieve::Process(float _DeltaTime)
{
	UPlayerData::Achieve(Achievement);
	return true;
}

bool ES::Unachieve::Process(float _DeltaTime)
{
	UPlayerData::Unachieve(Achievement);
	return true;
}

bool ES::GainItem::Process(float _DeltaTime)
{
	UPlayerData::GainItem(ItemId, Count);
	return true;
}

bool ES::End::Process(float _DeltaTime)
{
	return false;
}
