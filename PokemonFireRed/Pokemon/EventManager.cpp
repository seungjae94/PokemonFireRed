#include "EventManager.h"
#include <EngineCore/EngineCore.h>
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "EventCondition.h"
#include "Player.h"
#include "PokemonLevel.h"
#include "MenuWindow.h"
#include "DialogueWindow.h"

std::string UEventManager::CurLevelName;
std::map<std::string, APlayer*> UEventManager::AllPlayers;
std::map<std::string, AMenuWindow*> UEventManager::AllMenuWindows;
std::map<std::string, ADialogueWindow*> UEventManager::AllDialogueWindows;
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

// 이벤트 감지

void UEventManager::CheckPlayerEvent()
{
	APlayer* Player = AllPlayers[CurLevelName];
	AMenuWindow* MenuWindow = AllMenuWindows[CurLevelName];

	if (nullptr == Player)
	{
		// 플레이어가 없는 맵인 경우
		return;
	}

	// 플레이어가 이동 로직을 실행중이라면 이벤트를 감지하지 않는다.
	if (true == Player->IsExecutingMovingLogic)
	{
		return;
	}

	// 이미 이벤트를 실행 중이라면 새로운 이벤트를 실행하지 않는다.
	if (Player->State == EPlayerState::OutOfControl)
	{
		return;
	}

	// 클릭 이벤트 감지
	FTileVector CurPoint = FTileVector(Player->GetActorLocation());
	FTileVector TargetPoint = CurPoint + Player->Direction;
	if (true == AllTriggers[CurLevelName].contains(TargetPoint) && UEngineInput::IsDown('Z'))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][TargetPoint];
		UEventProcessor* Processor = AllProcessors[EventTrigger];

		bool RunResult = Processor->TryRun(EEventTriggerAction::Click);
		if (true == RunResult)
		{
			return;
		}
	}

	// Notice 이벤트 감지
	if (true == AllTriggers[CurLevelName].contains(TargetPoint))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][TargetPoint];
		UEventProcessor* Processor = AllProcessors[EventTrigger];
		
		bool RunResult = Processor->TryRun(EEventTriggerAction::Notice);
		if (true == RunResult)
		{
			return;
		}
	}

	// Step On 이벤트 감지
	if (true == AllTriggers[CurLevelName].contains(CurPoint))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][CurPoint];
		UEventProcessor* Processor = AllProcessors[EventTrigger];
		
		bool RunResult = Processor->TryRun(EEventTriggerAction::StepOn);
		if (true == RunResult)
		{
			return;
		}
	}

	// 메뉴창 열기 이벤트 감지
	if (true == UEngineInput::IsDown(VK_RETURN))
	{
		StealPlayerControl();
		MenuWindow->ActiveOn();
		MenuWindow->AllRenderersActiveOn();
	}
}

void UEventManager::Tick(float _DeltaTime)
{
	DeltaTime = _DeltaTime;

	CheckPlayerEvent();

	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair: AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (Processor->IsRunning())
		{
			Processor->Tick(_DeltaTime);
		}
	}
}

void UEventManager::Register(AEventTrigger* _Trigger, const UEventCondition& _Condition, Event _Event)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	Processor ->Register(_Condition, _Event);
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

void UEventManager::AddMenuWindow(AMenuWindow* _MenuWindow)
{
	std::string LevelName = _MenuWindow->GetWorld()->GetName();

	if (true == AllMenuWindows.contains(LevelName))
	{
		MsgBoxAssert("이미 등록된 메뉴창을 다시 등록하려고 했습니다.");
		return;
	}

	AllMenuWindows[LevelName] = _MenuWindow;
}

void UEventManager::AddDialogueWindow(ADialogueWindow* _DialogueWindow)
{
	std::string LevelName = _DialogueWindow->GetWorld()->GetName();

	if (true == AllDialogueWindows.contains(LevelName))
	{
		MsgBoxAssert("이미 등록된 메뉴창을 다시 등록하려고 했습니다.");
		return;
	}

	AllDialogueWindows[LevelName] = _DialogueWindow;
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

// 이벤트 함수

bool UEventManager::ChangeLevel(std::string_view _LevelName)
{
	CurLevelName = UEngineString::ToUpper(_LevelName);
	GEngine->UEngineCore::ChangeLevel(_LevelName);
	return true;
}

bool UEventManager::StealPlayerControl()
{
	APlayer* Player = AllPlayers[CurLevelName];
	Player->StateChange(EPlayerState::OutOfControl);
	return true;
}

bool UEventManager::GiveBackPlayerControl()
{
	APlayer* Player = AllPlayers[CurLevelName];
	Player->StateChange(EPlayerState::Idle);
	return true;
}

bool UEventManager::ChangeMap(std::string_view _NextMapName, const FTileVector& _Point)
{
	std::string CurMapName = UEngineString::ToUpper(CurLevelName);
	std::string NextMapName = UEngineString::ToUpper(_NextMapName);

	APlayer* NextMapPlayer = AllPlayers[NextMapName];

	if (nullptr == NextMapPlayer)
	{
		MsgBoxAssert("다음 맵에 플레이어가 존재하지 않습니다. 맵 이름 " + CurMapName + ", " + NextMapName + "을 제대로 입력했는지 확인하세요.");
		return false;
	}

	NextMapPlayer->StateChange(EPlayerState::OutOfControl);
	ChangePoint(NextMapName, NextMapPlayer->GetName(), _Point);

	UEventManager::ChangeLevel(NextMapName);

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

bool UEventManager::Chat(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, bool _IsSequential)
{
	ADialogueWindow* CurDialogueWindow = AllDialogueWindows[CurLevelName];

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
		CurDialogueWindow->SetDialogue(_Dialogue, _Color, _IsSequential);
		return false;
	}

	return false;
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