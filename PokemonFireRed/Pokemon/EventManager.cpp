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

void UEventManager::AddTarget(
	AEventTarget* _Target,
	std::string_view _Name,
	const FTileVector& _Point,
	const FTileVector& _Direction,
	bool _Rotatable,
	bool _Walkable
)
{
	std::string ReadableName = _Name.data();
	std::string UpperName = UEngineString::ToUpper(_Name);

	// ��� ���� �ʱ�ȭ
	_Target->SetName(UpperName);
	_Target->SetActorLocation(_Point.ToFVector());
	_Target->Rotatable = _Rotatable;
	_Target->Walkable = _Walkable;
	_Target->Direction = _Direction;

	std::string LevelName = _Target->GetWorld()->GetName();
	std::string TargetName = _Target->GetName();

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("�̹� ��ϵ� �̺�Ʈ Ÿ�� " + TargetName + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// ������ �ʱ�ȭ
	_Target->Renderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);
	UImageRenderer* Renderer = _Target->Renderer;
	Renderer->SetImage(ReadableName + "Idle.png");
	Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
	Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });
	
	// �ִϸ��̼� ����
	if (true == _Rotatable)
	{
		Renderer->CreateAnimation(ReadableName + "IdleDown", ReadableName + "Idle.png", 0, 0, 0.0f, false);
		Renderer->CreateAnimation(ReadableName + "IdleUp", ReadableName + "Idle.png", 1, 1, 0.0f, false);
		Renderer->CreateAnimation(ReadableName + "IdleLeft", ReadableName + "Idle.png", 2, 2, 0.0f, false);
		Renderer->CreateAnimation(ReadableName + "IdleRight", ReadableName + "Idle.png", 3, 3, 0.0f, false);
	}

	if (true == _Walkable)
	{
		float WalkInterval = Global::CHARACTER_WALK_ANIMATION_FRAME_INTERVAL;
		Renderer->CreateAnimation(ReadableName + "WalkDown", ReadableName + "WalkDown.png", 0, 3, WalkInterval, true);
		Renderer->CreateAnimation(ReadableName + "WalkUp", ReadableName + "WalkUp.png", 0, 3, WalkInterval, true);
		Renderer->CreateAnimation(ReadableName + "WalkLeft", ReadableName + "WalkLeft.png", 0, 3, WalkInterval, true);
		Renderer->CreateAnimation(ReadableName + "WalkRight", ReadableName + "WalkRight.png", 0, 3, WalkInterval, true);
	}
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
	AddTarget(_Player, "Player", _Point, FTileVector::Down, true, true);

	// �÷��̾� ������ ��� �ʱ�ȭ
	_Player->StateChange(EPlayerState::Idle);

	// �÷��̾� ���� �ִϸ��̼� ����
	float JumpInterval = Global::CHARACTER_JUMP_ANIMATION_FRAME_INTERVAL;
	_Player->Renderer->CreateAnimation("PlayerJumpDown", "PlayerJumpDown.png", 0, 52, JumpInterval, false);

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

	if (false == AllTargets.contains(MapName) || false == AllTargets[MapName].contains(TargetName))
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

		if (_Path[Target->MoveIndex + 1] == FTileVector::Zero)
		{
			MsgBoxAssert("MoveActor �Լ����� Path ���� FTileVector::Zero �Դϴ�.");
			return false;
		}

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

	if (nullptr == NextMapPlayer)
	{
		MsgBoxAssert("���� �ʿ� �÷��̾ �������� �ʽ��ϴ�. �� �̸� " + CurMapName + ", " + NextMapName + "�� ����� �Է��ߴ��� Ȯ���ϼ���.");
		return false;
	}

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
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� �̵���Ű���� �߽��ϴ�.");
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

bool UEventManager::ChangeDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	AEventTarget* Target = AllTargets[MapName][TargetName];

	if (nullptr == Target)
	{
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� ȸ����Ű���� �߽��ϴ�.");
		return false;
	}

	Target->SetDirection(_Direction);
	Target->ChangeAnimation(Target->GetMoveState(), _Direction);

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