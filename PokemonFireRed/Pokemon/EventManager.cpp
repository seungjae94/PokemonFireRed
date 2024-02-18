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

// 이벤트 감지

void UEventManager::CheckPlayerEvent()
{
	APlayer* Player = GetCurPlayer();
	AMenuWindow* MenuWindow = GetCurMenuWindow();

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
	if (Player->State == EPlayerState::OutOfControl || true == PlayerEventProcessing)
	{
		return;
	}

	// 클릭 이벤트 감지
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

	// Notice 이벤트 감지
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

	// Step On 이벤트 감지
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

	// 메뉴창 열기 이벤트 감지
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
			Player->GetWorld()->SetCameraPos(Player->GetActorLocation() - Global::HalfScreen);
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

void UEventManager::AddTarget(AEventTarget* _Target, const UEventTargetInit& _Setting)
{
	if (nullptr == _Target)
	{
		MsgBoxAssert("nullptr은 이벤트 타겟으로 추가할 수 없습니다.");
		return;
	}

	std::string TargetName = _Setting.Name;
	std::string ImageName = _Setting.ImageName;
	std::string LevelName = _Target->GetWorld()->GetName();

	// 멤버 변수 초기화
	_Target->SetName(TargetName);
	
	_Target->Point = _Setting.Point;						// 아직 등록하지 않은 타겟이라 SetPoint로 위치 설정이 불가능하다.
	_Target->SetActorLocation(_Setting.Point.ToFVector());	// 아직 등록하지 않은 타겟이라 SetPoint로 위치 설정이 불가능하다.

	_Target->Collidable = _Setting.Collidable;
	_Target->Rotatable = _Setting.Rotatable;
	_Target->Walkable = _Setting.Walkable;
	_Target->Direction = _Setting.Direction;
	_Target->HasImage = _Setting.HasImage;

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("이미 등록된 이벤트 타겟 " + TargetName + "을 다시 등록하려고 했습니다.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// 렌더러 초기화
	if (true == _Target->HasImage)
	{
		_Target->UpperBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Upper);
		_Target->LowerBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);

		UImageRenderer* UpperBodyRenderer = _Target->UpperBodyRenderer;
		UImageRenderer* LowerBodyRenderer = _Target->LowerBodyRenderer;

		UpperBodyRenderer->SetImage(ImageName);
		UpperBodyRenderer->SetTransform({ {0, -Global::TileSize}, {Global::TileSize, Global::TileSize} });
		UpperBodyRenderer->SetImageCuttingTransform({ {0, 0}, {Global::ImageTileSize, Global::ImageTileSize} });

		LowerBodyRenderer->SetImage(ImageName);
		LowerBodyRenderer->SetTransform({ {0, 0}, {Global::TileSize, Global::TileSize} });
		LowerBodyRenderer->SetImageCuttingTransform({ {0, 0}, {Global::ImageTileSize, Global::ImageTileSize} });

		// 애니메이션 생성
		std::vector<std::string> AllDirectionNames = FTileVector::AllDirectionNames();

		if (true == _Target->Rotatable)
		{
			int FrameIndex = 0;
			for (std::string& DirectionName : AllDirectionNames)
			{
				std::string ImageName = TargetName + "Idle.png";

				std::string UpperBodyAnimName = TargetName + "Idle" + DirectionName + Global::SuffixUpperBody;
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName, ImageName, FrameIndex, FrameIndex, 0.0f, false);

				std::string LowerBodyAnimName = TargetName + "Idle" + DirectionName + Global::SuffixLowerBody;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName, ImageName, FrameIndex + 4, FrameIndex + 4, 0.0f, false);

				FrameIndex++;
			}

			std::string TargetDirectionName = _Target->Direction.ToDirectionString();
			std::string InitialIdleAnimNamePrefix = TargetName + "Idle" + TargetDirectionName;
			UpperBodyRenderer->ChangeAnimation(InitialIdleAnimNamePrefix + Global::SuffixUpperBody);
			LowerBodyRenderer->ChangeAnimation(InitialIdleAnimNamePrefix + Global::SuffixLowerBody);
		}

		if (true == _Target->Walkable)
		{
			float WalkInterval = 1.0f / Global::CharacterWalkSpeed / 2;

			for (std::string& DirectionName : AllDirectionNames)
			{
				std::string ImageName = TargetName + "Walk" + DirectionName + ".png";

				std::string UpperBodyAnimName = TargetName + "Walk" + DirectionName + Global::SuffixUpperBody;
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "0", ImageName, {0, 1}, WalkInterval, false); // 오른발
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "1", ImageName, {2, 3}, WalkInterval, false); // 왼발

				std::string LowerBodyAnimName = TargetName + "Walk" + DirectionName + Global::SuffixLowerBody;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "0", ImageName, {4, 5}, WalkInterval, false); // 오른발
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "1", ImageName, {6, 7}, WalkInterval, false); // 왼발
			}
		}
	}

	// 콜리전 초기화
	if (true == _Target->Collidable)
	{
		ECollisionOrder Order = ECollisionOrder::NPC;

		if (nullptr != dynamic_cast<APlayer*>(_Target))
		{
			Order = ECollisionOrder::Player;
		}

		_Target->Collision = _Target->CreateCollision(Order);

		UCollision* Collision = _Target->Collision;
		Collision->SetColType(ECollisionType::Rect);
		Collision->SetScale({Global::FloatTileSize, Global::FloatTileSize });
	}
}

void UEventManager::AddTrigger(AEventTrigger* _Trigger, const UEventTargetInit& _Setting)
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
	NewProcessor->Trigger = _Trigger;
	AllProcessors[_Trigger] = NewProcessor;
}

void UEventManager::AddPlayer(APlayer* _Player, const FTileVector& _Point)
{
	UEventTargetInit PlayerSetting = UEventTargetInit(
		Global::PLAYER_NAME,
		_Point,
		FTileVector::Down,
		true,
		true,
		true,
		true
	);

	AddTarget(_Player, PlayerSetting);

	// 플레이어 고유의 멤버 초기화
	_Player->StateChange(EPlayerState::Idle);

	// 플레이어 점프 애니메이션 생성
	float JumpInterval = Global::CharacterJumpAnimFrameLength;
	_Player->UpperBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::SuffixUpperBody, 
		"PlayerJumpDown.png", 0, 52, JumpInterval, false);
	_Player->LowerBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::SuffixLowerBody, 
		"PlayerJumpDown.png", 53 + 0, 53 + 52, JumpInterval, false);

	std::string LevelName = _Player->GetWorld()->GetName();

	if (true == AllPlayers.contains(LevelName))
	{
		MsgBoxAssert("이미 등록된 플레이어를 다시 등록하려고 했습니다.");
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
		MsgBoxAssert("이미 등록된 UI 엘리먼트" + Name + "을 다시 등록하려고 했습니다.");
		return;
	}

	AllUIElements[LevelName][Name] = _UIElement;
}

// 이벤트 구현

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
		MsgBoxAssert("존재하지 않는 타겟" + MapName + ":" + TargetName + "을 이동시키려고 했습니다.");
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
		MsgBoxAssert("존재하지 않는 타겟" + MapName + ":" + TargetName + "을 회전시키려고 했습니다.");
		return;
	}

	Target->SetDirection(_Direction);
	Target->ChangeAnimation(Target->GetMoveState(), _Direction);
}


// 찾기 편의 함수

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