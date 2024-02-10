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
		MsgBoxAssert(_Point.ToString() + "위치에는 이벤트 트리거가 존재하지 않습니다. 트리거를 할 수 없습니다.");
		return;
	}

	AEventTrigger* EventTrigger = AllTriggers[LevelName][_Point];
	UEventProcessor* Processor = AllProcessors[EventTrigger];
	Processor->Work();
}

void UEventManager::AddTarget(AEventTarget* _Target, const UEventTargetInitialSetting& _Setting)
{
	std::string TargetName = _Setting.Name;
	std::string ImageName = _Setting.ImageName;

	// 멤버 변수 초기화
	_Target->SetName(TargetName);
	_Target->SetActorLocation(_Setting.Point.ToFVector());
	_Target->Rotatable = _Setting.Rotatable;
	_Target->Walkable = _Setting.Walkable;
	_Target->Direction = _Setting.Direction;
	_Target->HasImage = _Setting.HasImage;

	std::string LevelName = _Target->GetWorld()->GetName();

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("이미 등록된 이벤트 타겟 " + TargetName + "을 다시 등록하려고 했습니다.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// 렌더러 초기화
	if (true == _Target->HasImage)
	{
		_Target->Renderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);

		UImageRenderer* Renderer = _Target->Renderer;
		Renderer->SetImage(ImageName);
		Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
		Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });

		// 애니메이션 생성
		if (true == _Target->Rotatable)
		{
			Renderer->CreateAnimation(TargetName + "IdleDown", TargetName + "Idle.png", 0, 0, 0.0f, false);
			Renderer->CreateAnimation(TargetName + "IdleUp", TargetName + "Idle.png", 1, 1, 0.0f, false);
			Renderer->CreateAnimation(TargetName + "IdleLeft", TargetName + "Idle.png", 2, 2, 0.0f, false);
			Renderer->CreateAnimation(TargetName + "IdleRight", TargetName + "Idle.png", 3, 3, 0.0f, false);

			Renderer->ChangeAnimation(TargetName + "Idle" + _Target->Direction.ToDirectionString());
		}

		if (true == _Target->Walkable)
		{
			float WalkInterval = Global::CHARACTER_WALK_ANIMATION_FRAME_INTERVAL;
			Renderer->CreateAnimation(TargetName + "WalkDown", TargetName + "WalkDown.png", 0, 3, WalkInterval, true);
			Renderer->CreateAnimation(TargetName + "WalkUp", TargetName + "WalkUp.png", 0, 3, WalkInterval, true);
			Renderer->CreateAnimation(TargetName + "WalkLeft", TargetName + "WalkLeft.png", 0, 3, WalkInterval, true);
			Renderer->CreateAnimation(TargetName + "WalkRight", TargetName + "WalkRight.png", 0, 3, WalkInterval, true);
		}
	}
}

void UEventManager::AddTrigger(AEventTrigger* _Trigger, const UEventTargetInitialSetting& _Setting)
{
	std::string TriggerName = _Setting.Name;
	FTileVector Point = _Setting.Point;
	std::string LevelName = _Trigger->GetWorld()->GetName();

	if (true == AllTriggers[LevelName].contains(Point))
	{
		MsgBoxAssert("이미 트리거가 존재하는 위치에 트리거" + TriggerName + "을 등록하려고 했습니다.");
		return;
	}

	AllTriggers[LevelName][Point] = _Trigger;

	if (true == AllProcessors.contains(_Trigger))
	{
		MsgBoxAssert("이미 등록된 이벤트 트리거 " + TriggerName + "을 다시 등록하려고 했습니다.");
		return;
	}

	UEventProcessor* NewProcessor = new UEventProcessor();
	AllProcessors[_Trigger] = NewProcessor;
}

void UEventManager::AddPlayer(APlayer* _Player, const FTileVector& _Point)
{
	UEventTargetInitialSetting PlayerSetting = UEventTargetInitialSetting(
		"Player",
		_Point,
		FTileVector::Down,
		true,
		true,
		true
	);

	AddTarget(_Player, PlayerSetting);

	// 플레이어 고유의 멤버 초기화
	_Player->StateChange(EPlayerState::Idle);

	// 플레이어 점프 애니메이션 생성
	float JumpInterval = Global::CHARACTER_JUMP_ANIMATION_FRAME_INTERVAL;
	_Player->Renderer->CreateAnimation("PlayerJumpDown", "PlayerJumpDown.png", 0, 52, JumpInterval, false);

	std::string LevelName = _Player->GetWorld()->GetName();

	if (true == AllPlayers.contains(LevelName))
	{
		MsgBoxAssert("이미 등록된 플레이어를 다시 등록하려고 했습니다.");
		return;
	}

	AllPlayers[LevelName] = _Player;
}

// 이벤트 구현

bool UEventManager::MoveActor(std::string_view _MapName, std::string_view _TargetName, std::vector<FTileVector> _Path, float _MoveSpeed)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	if (_Path.size() <= 0)
	{
		MsgBoxAssert("강제 이동 경로의 크기가 0 이하입니다.");
		return false;
	}

	if (false == AllTargets.contains(MapName) || false == AllTargets[MapName].contains(TargetName))
	{
		MsgBoxAssert(MapName + ":" + TargetName + "는 존재하지 않는 이벤트 타겟입니다.존재하지 않는 이벤트 타겟을 이동시키려고 했습니다.");
		return false;
	}

	AEventTarget* Target = AllTargets[MapName][TargetName];

	// 이동 이벤트 시작
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
			MsgBoxAssert("MoveActor 함수에서 Path 값이 FTileVector::Zero 입니다.");
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
		MsgBoxAssert("다음 맵에 플레이어가 존재하지 않습니다. 맵 이름 " + CurMapName + ", " + NextMapName + "을 제대로 입력했는지 확인하세요.");
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
		MsgBoxAssert("존재하지 않는 타겟" + MapName + ":" + TargetName + "을 이동시키려고 했습니다.");
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
		MsgBoxAssert("존재하지 않는 타겟" + MapName + ":" + TargetName + "을 회전시키려고 했습니다.");
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

// 메모리 릴리즈

void UEventManager::Release()
{
	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair : AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (nullptr == Processor)
		{
			MsgBoxAssert("이벤트 프로세서가 이미 삭제되어 있습니다.");
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