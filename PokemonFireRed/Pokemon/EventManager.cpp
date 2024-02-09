#include "EventManager.h"
#include <EngineCore/EngineCore.h>
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "Player.h"

std::map<std::string, APlayer*> UEventManager::AllPlayers;
std::map<std::string, std::map<std::string, AEventTarget*>> UEventManager::AllTargets;
std::map<std::string, std::map<FTileVector, AEventTrigger*>> UEventManager::AllTriggers;
std::map<AEventTrigger*, UEventProcessor*> UEventManager::AllProcessors;
float UEventManager::DeltaTime = 0.0f;

UEventManager::UEventManager()
{
}

UEventManager::~UEventManager()
{
	
}

void UEventManager::Tick(float _DeltaTime)
{
	DeltaTime = _DeltaTime;

	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair: AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (Processor->IsWorking())
		{
			Processor->Tick(_DeltaTime);
		}
	}
}

void UEventManager::Register(AEventTrigger* _Trigger, Event _Event)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	Processor->Register(_Event);
}

bool UEventManager::IsTrigger(std::string_view _LevelName, const FTileVector& _Point)
{
	std::string LevelName = UEngineString::ToUpper(_LevelName);
	return AllTriggers[LevelName].contains(_Point);
}

void UEventManager::Trigger(std::string_view _LevelName, const FTileVector& _Point)
{
	std::string LevelName = UEngineString::ToUpper(_LevelName);

	if (false == IsTrigger(LevelName, _Point))
	{
		MsgBoxAssert(_Point.ToString() + "��ġ���� �̺�Ʈ Ʈ���Ű� �������� �ʽ��ϴ�. Ʈ���Ÿ� �� �� �����ϴ�.");
		return;
	}

	AEventTrigger* EventTrigger = AllTriggers[LevelName][_Point];
	UEventProcessor* Processor = AllProcessors[EventTrigger];
	Processor->Work();
}

void UEventManager::AddTarget(AEventTarget* _Target, std::string_view _Name, const FTileVector& _Point)
{
	std::string Name = UEngineString::ToUpper(_Name);

	_Target->SetName(Name);
	_Target->SetActorLocation(_Point.ToFVector());

	std::string LevelName = _Target->GetWorld()->GetName();
	std::string TargetName = _Target->GetName();

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("�̹� ��ϵ� �̺�Ʈ Ÿ�� " + TargetName + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;
}

void UEventManager::AddTrigger(AEventTrigger* _Trigger, std::string_view _Name, const FTileVector& _Point)
{
	std::string Name = UEngineString::ToUpper(_Name);

	_Trigger->SetName(Name);
	_Trigger->SetActorLocation(_Point.ToFVector());

	std::string LevelName = _Trigger->GetWorld()->GetName();
	std::string TriggerName = _Trigger->GetName();

	if (true == AllTriggers[LevelName].contains(_Point))
	{
		MsgBoxAssert("�̹� Ʈ���Ű� �����ϴ� ��ġ�� Ʈ����" + TriggerName + "�� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTriggers[LevelName][_Point] = _Trigger;

	if (true == AllProcessors.contains(_Trigger))
	{
		MsgBoxAssert("�̹� ��ϵ� �̺�Ʈ Ʈ���� " + TriggerName + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	UEventProcessor* NewProcessor = new UEventProcessor();
	AllProcessors[_Trigger] = NewProcessor;
}

void UEventManager::AddPlayer(APlayer* _Player, const FTileVector& _Point)
{
	AddTarget(_Player, "Player", _Point);
	_Player->StateChange(EPlayerState::Idle);
	_Player->SetDirection(FTileVector::Down);

	std::string LevelName = _Player->GetWorld()->GetName();

	if (true == AllPlayers.contains(LevelName))
	{
		MsgBoxAssert("�̹� ��ϵ� �÷��̾ �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllPlayers[LevelName] = _Player;
}

// �̺�Ʈ ����

bool UEventManager::MoveActor(std::string_view _MapName, std::string_view _TargetName, std::vector<FTileVector> _Path, float _MoveSpeed)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	if (_Path.size() <= 0)
	{
		MsgBoxAssert("���� �̵� ����� ũ�Ⱑ 0 �����Դϴ�.");
		return false;
	}

	if (false == AllTargets[MapName].contains(TargetName))
	{
		MsgBoxAssert(MapName + ":" + TargetName + "�� �������� �ʴ� �̺�Ʈ Ÿ���Դϴ�.�������� �ʴ� �̺�Ʈ Ÿ���� �̵���Ű���� �߽��ϴ�.");
		return false;
	}

	AEventTarget* Target = AllTargets[MapName][TargetName];

	// �̵� �̺�Ʈ ����
	if (Target->MoveIndex == -1)
	{
		Target->MoveTime = 1.0f / _MoveSpeed;
		Target->Timer = 0.0f;
		Target->SetMoveState(ETargetMoveState::Walk);
		Target->ChangeAnimation(Target->MoveState, Target->Direction);
	}

	if (Target->Timer > 0.0f)
	{
		Target->Timer -= DeltaTime;

		float t = (Target->MoveTime - Target->Timer) / Target->MoveTime;

		FVector TargetPos = PokemonMath::Lerp(Target->PrevPos, Target->NextPos, t);
		FVector PlayerPos = Target->GetActorLocation();
		FVector AddPos = TargetPos - PlayerPos;
		Target->AddActorLocation(AddPos);
		Target->GetWorld()->SetCameraPos(Target->GetActorLocation() - Global::HALF_SCREEN);
	}
	else if (Target->MoveIndex + 1 >= _Path.size())
	{
		Target->MoveIndex = -1;
		Target->SetMoveState(ETargetMoveState::Idle);
		Target->ChangeAnimation(Target->MoveState, Target->Direction);
		return true;
	}
	else
	{
		Target->PrevPos = Target->GetActorLocation();
		Target->NextPos = Target->PrevPos + _Path[Target->MoveIndex + 1].ToFVector();
		Target->Timer = Target->MoveTime;
		if (Target->Direction != _Path[Target->MoveIndex + 1])
		{
			Target->SetDirection(_Path[Target->MoveIndex + 1]);
			Target->ChangeAnimation(Target->MoveState, Target->Direction);
		}
		Target->MoveIndex++;
	}
	return false;
}

bool UEventManager::ChangeMap(std::string_view _CurMapName, std::string_view _NextMapName, const FTileVector& _Point)
{
	std::string CurMapName = UEngineString::ToUpper(_CurMapName);
	std::string NextMapName = UEngineString::ToUpper(_NextMapName);

	APlayer* NextMapPlayer = AllPlayers[NextMapName];
	NextMapPlayer->StateChange(EPlayerState::Event);
	ChangePoint(NextMapName, NextMapPlayer->GetName(), _Point);

	GEngine->ChangeLevel(NextMapName);

	return true;
}

bool UEventManager::ChangePoint(std::string_view _MapName, std::string_view _TargetName , const FTileVector& _Point)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	AEventTarget* Target = AllTargets[MapName][TargetName];

	if (nullptr == Target)
	{
		MsgBoxAssert("�������� �ʴ� Ÿ��" + TargetName + "�� �̵��ø��Ѱ� �߽��ϴ�.");
		return false;
	}

	FVector TargetPosition = _Point.ToFVector();
	Target->SetActorLocation(TargetPosition);

	if (true == AllTriggers[MapName].contains(_Point))
	{
		AEventTrigger* Trigger = AllTriggers[MapName][_Point];

		std::map<FTileVector, AEventTrigger*>::iterator FindIter = AllTriggers[MapName].find(_Point);
		AllTriggers[MapName].erase(FindIter);

		AllTriggers[MapName][FTileVector(TargetPosition)] = Trigger;
	}

	return true;
}

bool UEventManager::Finish(std::string_view _LevelName)
{
	std::string LevelName = UEngineString::ToUpper(_LevelName);

	APlayer* Player = AllPlayers[LevelName];
	Player->StateChange(EPlayerState::Idle);

	return true;
}

// �޸� ������

void UEventManager::Release()
{
	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair : AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (nullptr == Processor)
		{
			MsgBoxAssert("�̺�Ʈ ���μ����� �̹� �����Ǿ� �ֽ��ϴ�.");
			return;
		}

		delete Processor;
		Pair.second = nullptr;
	}

	AllProcessors.clear();
}

class UEventManagerReleaser
{
public:
	~UEventManagerReleaser()
	{
		UEventManager::Release();
	}
};
UEventManagerReleaser UEventManagerReleaserObject;