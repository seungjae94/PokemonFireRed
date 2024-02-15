#include "EventManager.h"
#include <EngineCore/EngineCore.h>
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"
#include "PokemonLevel.h"
#include "MenuWindow.h"
#include "DialogueWindow.h"
#include "BlackScreen.h"

bool UEventManager::CameraFollowing = true;
bool UEventManager::PlayerEventProcessing = false;
std::string UEventManager::CurLevelName;
std::map<std::string, APlayer*> UEventManager::AllPlayers;
std::map<std::string, std::map<std::string, AUIElement*>> UEventManager::AllUIElements;
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
	APlayer* Player = GetCurPlayer();
	AMenuWindow* MenuWindow = GetCurMenuWindow();

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
	if (Player->State == EPlayerState::OutOfControl || true == PlayerEventProcessing)
	{
		return;
	}

	// Ŭ�� �̺�Ʈ ����
	FTileVector CurPoint = FTileVector(Player->GetActorLocation());
	FTileVector TargetPoint = CurPoint + Player->Direction;
	if (true == AllTriggers[CurLevelName].contains(TargetPoint) && UEngineInput::IsDown('Z'))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][TargetPoint];

		bool RunResult = TriggerEvent(EventTrigger, EEventTriggerAction::Click);
		if (true == RunResult)
		{
			PlayerEventProcessing = true;
			return;
		}
	}

	// Notice �̺�Ʈ ����
	if (true == AllTriggers[CurLevelName].contains(TargetPoint))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][TargetPoint];
		
		bool RunResult = TriggerEvent(EventTrigger, EEventTriggerAction::Notice);
		if (true == RunResult)
		{
			PlayerEventProcessing = true;
			return;
		}
	}

	// Step On �̺�Ʈ ����
	if (true == AllTriggers[CurLevelName].contains(CurPoint))
	{
		AEventTrigger* EventTrigger = AllTriggers[CurLevelName][CurPoint];

		bool RunResult = TriggerEvent(EventTrigger, EEventTriggerAction::StepOn);
		if (true == RunResult)
		{
			PlayerEventProcessing = true;
			return;
		}
	}

	// �޴�â ���� �̺�Ʈ ����
	if (true == UEngineInput::IsDown(VK_RETURN))
	{
		PlayerEventProcessing = true;
		MenuWindow->Open();
	}
}

void UEventManager::Tick(float _DeltaTime)
{
	DeltaTime = _DeltaTime;

	CheckPlayerEvent();

	if (true == CameraFollowing)
	{
		APlayer* Player = GetCurPlayer();

		if (nullptr != Player)
		{
			Player->GetWorld()->SetCameraPos(Player->GetActorLocation() - Global::HALF_SCREEN);
		}
	}

	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair: AllProcessors)
	{
		UEventProcessor* Processor = Pair.second;

		if (Processor->IsRunning())
		{
			Processor->Tick(_DeltaTime);
		}
	}
}

void UEventManager::RegisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition, UEventStream _Stream)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	Processor->RegisterStream(_Condition, _Stream);
}

bool UEventManager::TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	return Processor->TryRun(_Action);
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
		_Target->UpperBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Upper);
		_Target->LowerBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);

		UImageRenderer* UpperBodyRenderer = _Target->UpperBodyRenderer;
		UImageRenderer* LowerBodyRenderer = _Target->LowerBodyRenderer;

		UpperBodyRenderer->SetImage(ImageName);
		UpperBodyRenderer->SetTransform({ {0, -Global::TILE_SIZE}, {Global::TILE_SIZE, Global::TILE_SIZE} });
		UpperBodyRenderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, Global::IMAGE_TILE_SIZE} });

		LowerBodyRenderer->SetImage(ImageName);
		LowerBodyRenderer->SetTransform({ {0, 0}, {Global::TILE_SIZE, Global::TILE_SIZE} });
		LowerBodyRenderer->SetImageCuttingTransform({ {0, 0}, {Global::IMAGE_TILE_SIZE, Global::IMAGE_TILE_SIZE} });

		// �ִϸ��̼� ����
		std::vector<std::string> AllDirectionNames = FTileVector::AllDirectionNames();

		if (true == _Target->Rotatable)
		{
			int FrameIndex = 0;
			for (std::string& DirectionName : AllDirectionNames)
			{
				std::string ImageName = TargetName + "Idle.png";

				std::string UpperBodyAnimName = TargetName + "Idle" + DirectionName + Global::ANIMATION_NAME_SUFFIX_UPPER_BODY;
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName, ImageName, FrameIndex, FrameIndex, 0.0f, false);

				std::string LowerBodyAnimName = TargetName + "Idle" + DirectionName + Global::ANIMATION_NAME_SUFFIX_LOWER_BODY;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName, ImageName, FrameIndex + 4, FrameIndex + 4, 0.0f, false);

				FrameIndex++;
			}

			std::string TargetDirectionName = _Target->Direction.ToDirectionString();
			std::string InitialIdleAnimNamePrefix = TargetName + "Idle" + TargetDirectionName;
			UpperBodyRenderer->ChangeAnimation(InitialIdleAnimNamePrefix + Global::ANIMATION_NAME_SUFFIX_UPPER_BODY);
			LowerBodyRenderer->ChangeAnimation(InitialIdleAnimNamePrefix + Global::ANIMATION_NAME_SUFFIX_LOWER_BODY);
		}

		if (true == _Target->Walkable)
		{
			float WalkInterval = Global::CHARACTER_WALK_ANIMATION_FRAME_INTERVAL;

			for (std::string& DirectionName : AllDirectionNames)
			{
				std::string ImageName = TargetName + "Walk" + DirectionName + ".png";

				std::string UpperBodyAnimName = TargetName + "Walk" + DirectionName + Global::ANIMATION_NAME_SUFFIX_UPPER_BODY;
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName, ImageName, {1, 2, 3, 0}, WalkInterval, true);

				std::string LowerBodyAnimName = TargetName + "Walk" + DirectionName + Global::ANIMATION_NAME_SUFFIX_LOWER_BODY;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName, ImageName, {5, 6, 7, 4}, WalkInterval, true);
			}
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
	_Player->UpperBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::ANIMATION_NAME_SUFFIX_UPPER_BODY, 
		"PlayerJumpDown.png", 0, 52, JumpInterval, false);
	_Player->LowerBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::ANIMATION_NAME_SUFFIX_LOWER_BODY, 
		"PlayerJumpDown.png", 53 + 0, 53 + 52, JumpInterval, false);

	std::string LevelName = _Player->GetWorld()->GetName();

	if (true == AllPlayers.contains(LevelName))
	{
		MsgBoxAssert("�̹� ��ϵ� �÷��̾ �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllPlayers[LevelName] = _Player;
}

void UEventManager::AddUIElement(AUIElement* _UIElement, std::string_view _Name)
{
	std::string LevelName = _UIElement->GetWorld()->GetName();
	std::string Name = _Name.data();

	if (true == AllUIElements[LevelName].contains(Name))
	{
		MsgBoxAssert("�̹� ��ϵ� UI ������Ʈ" + Name + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllUIElements[LevelName][Name] = _UIElement;
}

// �̺�Ʈ ����

void UEventManager::SetLevel(std::string_view _LevelName)
{
	CurLevelName = UEngineString::ToUpper(_LevelName);
	GEngine->UEngineCore::ChangeLevel(_LevelName);
}

void UEventManager::SetPoint(std::string_view _MapName, std::string_view _TargetName , const FTileVector& _Point)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	AEventTarget* Target = AllTargets[MapName][TargetName];

	if (nullptr == Target)
	{
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� �̵���Ű���� �߽��ϴ�.");
		return;
	}

	FVector TargetPosition = _Point.ToFVector();
	Target->SetActorLocation(TargetPosition);
	Target->Point = _Point;

	if (true == AllTriggers[MapName].contains(_Point))
	{
		AEventTrigger* Trigger = AllTriggers[MapName][_Point];

		std::map<FTileVector, AEventTrigger*>::iterator FindIter = AllTriggers[MapName].find(_Point);
		AllTriggers[MapName].erase(FindIter);

		AllTriggers[MapName][FTileVector(TargetPosition)] = Trigger;
	}
}

void UEventManager::SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	AEventTarget* Target = AllTargets[MapName][TargetName];

	if (nullptr == Target)
	{
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� ȸ����Ű���� �߽��ϴ�.");
		return;
	}

	Target->SetDirection(_Direction);
	Target->ChangeAnimation(Target->GetMoveState(), _Direction);
}


// ã�� ���� �Լ�

APlayer* UEventManager::GetCurPlayer()
{
	return AllPlayers[CurLevelName];
}

AMenuWindow* UEventManager::GetCurMenuWindow()
{
	return dynamic_cast<AMenuWindow*>(AllUIElements[CurLevelName]["MenuWindow"]);
}

ADialogueWindow* UEventManager::GetCurDialogueWindow()
{
	return dynamic_cast<ADialogueWindow*>(AllUIElements[CurLevelName]["DialogueWindow"]);
}

ABlackScreen* UEventManager::GetCurBlackScreen()
{
	return dynamic_cast<ABlackScreen*>(AllUIElements[CurLevelName]["BlackScreen"]);
}

ABlackScreen* UEventManager::GetBlackScreen(std::string_view _LevelName)
{
	return dynamic_cast<ABlackScreen*>(AllUIElements[_LevelName.data()]["BlackScreen"]);
}

AEventTarget* UEventManager::FindTarget(std::string_view _LevelName, std::string_view _TargetName)
{
	if (false == AllTargets.contains(_LevelName.data()) || false == AllTargets[_LevelName.data()].contains(_TargetName.data()))
	{
		return nullptr;
	}

	return AllTargets[_LevelName.data()][_TargetName.data()];
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