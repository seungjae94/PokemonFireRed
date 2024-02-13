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

// �̺�Ʈ ����

void UEventManager::CheckPlayerEvent()
{
	APlayer* Player = AllPlayers[CurLevelName];
	AMenuWindow* MenuWindow = AllMenuWindows[CurLevelName];

	if (nullptr == Player)
	{
		// �÷��̾ ���� ���� ���
		return;
	}

	// �÷��̾ �̵� ������ �������̶�� �̺�Ʈ�� �������� �ʴ´�.
	if (true == Player->IsExecutingMovingLogic)
	{
		return;
	}

	// �̹� �̺�Ʈ�� ���� ���̶�� ���ο� �̺�Ʈ�� �������� �ʴ´�.
	if (Player->State == EPlayerState::OutOfControl)
	{
		return;
	}

	// Ŭ�� �̺�Ʈ ����
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

	// Notice �̺�Ʈ ����
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

	// Step On �̺�Ʈ ����
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

	// �޴�â ���� �̺�Ʈ ����
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

	// ��� ���� �ʱ�ȭ
	_Target->SetName(TargetName);
	_Target->SetActorLocation(_Setting.Point.ToFVector());
	_Target->Rotatable = _Setting.Rotatable;
	_Target->Walkable = _Setting.Walkable;
	_Target->Direction = _Setting.Direction;
	_Target->HasImage = _Setting.HasImage;

	std::string LevelName = _Target->GetWorld()->GetName();

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("�̹� ��ϵ� �̺�Ʈ Ÿ�� " + TargetName + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// ������ �ʱ�ȭ
	if (true == _Target->HasImage)
	{
		_Target->Renderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);

		UImageRenderer* Renderer = _Target->Renderer;
		Renderer->SetImage(ImageName);
		Renderer->SetTransform({ {0, -Global::TILE_SIZE / 2}, {Global::TILE_SIZE, 2 * Global::TILE_SIZE} });
		Renderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, 2 * Global::IMAGE_TILE_SIZE} });

		// �ִϸ��̼� ����
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
		MsgBoxAssert("�̹� Ʈ���Ű� �����ϴ� ��ġ�� Ʈ����" + TriggerName + "�� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTriggers[LevelName][Point] = _Trigger;

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
	UEventTargetInitialSetting PlayerSetting = UEventTargetInitialSetting(
		"Player",
		_Point,
		FTileVector::Down,
		true,
		true,
		true
	);

	AddTarget(_Player, PlayerSetting);

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

void UEventManager::AddMenuWindow(AMenuWindow* _MenuWindow)
{
	std::string LevelName = _MenuWindow->GetWorld()->GetName();

	if (true == AllMenuWindows.contains(LevelName))
	{
		MsgBoxAssert("�̹� ��ϵ� �޴�â�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllMenuWindows[LevelName] = _MenuWindow;
}

void UEventManager::AddDialogueWindow(ADialogueWindow* _DialogueWindow)
{
	std::string LevelName = _DialogueWindow->GetWorld()->GetName();

	if (true == AllDialogueWindows.contains(LevelName))
	{
		MsgBoxAssert("�̹� ��ϵ� �޴�â�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllDialogueWindows[LevelName] = _DialogueWindow;
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

// �̺�Ʈ �Լ�

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
		MsgBoxAssert("���� �ʿ� �÷��̾ �������� �ʽ��ϴ�. �� �̸� " + CurMapName + ", " + NextMapName + "�� ����� �Է��ߴ��� Ȯ���ϼ���.");
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