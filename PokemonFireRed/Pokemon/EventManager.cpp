#include "EventManager.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineResourcesManager.h>
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "Player.h"
#include "MapLevel.h"
#include "MenuCanvas.h"
#include "DialogueWindow.h"
#include "WildBattleTrigger.h"
#include "Trainer.h"

std::string UEventManager::CurLevelName;
ULevel* UEventManager::BattleLevel = nullptr;
std::map<std::string, std::map<std::string, ACanvas*>> UEventManager::AllCommonCanvas;
std::map<std::string, ADialogueWindow*> UEventManager::AllDialogueWindows;
std::map<std::string, std::map<std::string, AEventTarget*>> UEventManager::AllTargets;
std::map<std::string, std::map<FTileVector, std::list<AEventTrigger*>>> UEventManager::AllTriggers;
std::map<AEventTrigger*, UEventProcessor*> UEventManager::AllProcessors;
bool UEventManager::IsWildPokemon = false;
std::vector<UPokemon>* UEventManager::EnemyEntry;
ATrainer* UEventManager::Trainer;
float UEventManager::DeltaTime = 0.0f;

UEventManager::UEventManager()
{
}

UEventManager::~UEventManager()
{

}

// 이벤트 감지
void UEventManager::Tick(float _DeltaTime)
{
	DeltaTime = _DeltaTime;

	for (std::pair<AEventTrigger* const, UEventProcessor*>& Pair : AllProcessors)
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

void UEventManager::UnregisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	Processor->UnregisterStream(_Condition);
}

bool UEventManager::TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action)
{
	UEventProcessor* Processor = AllProcessors[_Trigger];
	return Processor->TryRun(_Action);
}

void UEventManager::SaveEnemyEntry(std::vector<UPokemon>* _Entry)
{
	EnemyEntry = _Entry;
}

std::vector<UPokemon>* UEventManager::LoadEnemyEntry()
{
	return EnemyEntry;
}

bool UEventManager::IsWildPokemonBattle()
{
	return IsWildPokemon;
}

void UEventManager::SetAsWildPokemonBattle()
{
	IsWildPokemon = true;
}

void UEventManager::SetAsTrainerBattle(ATrainer* _Trainer)
{
	IsWildPokemon = false;
	Trainer = _Trainer;
}

void UEventManager::AddTarget(AEventTarget* _Target, const UEventTargetSetting& _Setting)
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
	_Target->ImageName = _Setting.ImageName;
	_Target->Height = _Setting.Height;

	if (_Target->ImageName != "")
	{
		_Target->HasImage = true;
	}

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("이미 등록된 이벤트 타겟 " + TargetName + "을 다시 등록하려고 했습니다.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// 렌더러 초기화
	if (true == _Target->HasImage)
	{
		UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(ImageName);
		if (nullptr == Image)
		{
			MsgBoxAssert(ImageName + "은 존재하지 않는 이미지입니다. 이벤트 타겟 " + TargetName + " 생성에 실패했습니다.");
			return;
		}

		UImageRenderer* LowerBodyRenderer = nullptr;
		UImageRenderer* UpperBodyRenderer = nullptr;

		_Target->LowerBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);
		LowerBodyRenderer = _Target->LowerBodyRenderer;
		LowerBodyRenderer->SetImage(ImageName);
		LowerBodyRenderer->SetTransform({ {0, 0}, {Global::TileSize, Global::TileSize} });

		// 위아래 2칸 이미지인 경우에만 UpperBody를 렌더링한다.
		if (_Target->Height == 2)
		{
			_Target->UpperBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Upper);
			UpperBodyRenderer = _Target->UpperBodyRenderer;
			UpperBodyRenderer->SetImage(ImageName);
			UpperBodyRenderer->SetTransform({ {0, -Global::TileSize}, {Global::TileSize, Global::TileSize} });
		}

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
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "0", ImageName, { 0, 1 }, WalkInterval, false); // 오른발
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "1", ImageName, { 2, 3 }, WalkInterval, false); // 왼발

				std::string LowerBodyAnimName = TargetName + "Walk" + DirectionName + Global::SuffixLowerBody;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "0", ImageName, { 4, 5 }, WalkInterval, false); // 오른발
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "1", ImageName, { 6, 7 }, WalkInterval, false); // 왼발
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
		Collision->SetScale({ Global::FloatTileSize, Global::FloatTileSize });
	}
}

void UEventManager::AddTrigger(AEventTrigger* _Trigger, const UEventTargetSetting& _Setting)
{
	std::string TriggerName = _Setting.Name;
	FTileVector Point = _Setting.Point;
	std::string LevelName = _Trigger->GetWorld()->GetName();

	AllTriggers[LevelName][Point].push_back(_Trigger);

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
	UEventTargetSetting PlayerSetting;
	PlayerSetting.SetName(Global::Player);
	PlayerSetting.SetPoint(_Point);
	PlayerSetting.SetDirection(FTileVector::Down);
	PlayerSetting.SetCollidable(true);
	PlayerSetting.SetRotatable(true);
	PlayerSetting.SetWalkable(true);
	PlayerSetting.SetImageNameAuto();

	AddTarget(_Player, PlayerSetting);

	// 플레이어 고유의 멤버 초기화
	_Player->StateChange(EPlayerState::Idle);

	// 플레이어 애니메이션 생성
	std::vector<std::string> AllDirectionNames = FTileVector::AllDirectionNames();

	// 애니메이션 - 점프
	float JumpInterval = Global::CharacterJumpAnimFrameLength;
	_Player->UpperBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::SuffixUpperBody,
		"PlayerJumpDown.png", 0, 52, JumpInterval, false);
	_Player->LowerBodyRenderer->CreateAnimation("PlayerJumpDown" + Global::SuffixLowerBody,
		"PlayerJumpDown.png", 53 + 0, 53 + 52, JumpInterval, false);

	// 애니메이션 - 느리게 걷기
	float WalkInterval = 1.0f / Global::CharacterWalkSpeed / 2;
	float SlowWalkInterval = WalkInterval * 2.0f;

	for (std::string& DirectionName : AllDirectionNames)
	{
		std::string ImageName = Global::Player + "Walk" + DirectionName + ".png";

		std::string UpperBodyAnimName = Global::Player + "SlowWalk" + DirectionName + Global::SuffixUpperBody;
		_Player->UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "0", ImageName, { 0, 1 }, SlowWalkInterval, false); // 오른발
		_Player->UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "1", ImageName, { 2, 3 }, SlowWalkInterval, false); // 왼발

		std::string LowerBodyAnimName = Global::Player + "SlowWalk" + DirectionName + Global::SuffixLowerBody;
		_Player->LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "0", ImageName, { 4, 5 }, SlowWalkInterval, false); // 오른발
		_Player->LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "1", ImageName, { 6, 7 }, SlowWalkInterval, false); // 왼발
	}
}

void UEventManager::AddCommonCanvas(ACanvas* _UIElement, std::string_view _Name)
{
	std::string LevelName = UEngineString::ToUpper(_UIElement->GetWorld()->GetName());
	std::string Name = UEngineString::ToUpper(_Name.data());

	if (true == AllCommonCanvas[LevelName].contains(Name))
	{
		MsgBoxAssert("이미 등록된 UI 엘리먼트" + Name + "을 다시 등록하려고 했습니다.");
		return;
	}

	AllCommonCanvas[LevelName][Name] = _UIElement;
}

void UEventManager::AddDialogueWindow(ADialogueWindow* _Window)
{
	std::string LevelName = UEngineString::ToUpper(_Window->GetWorld()->GetName());
	std::string Name = UEngineString::ToUpper(Global::DialogueWindow);

	if (true == AllDialogueWindows.contains(LevelName))
	{
		MsgBoxAssert("이미 등록된 대화창을 다시 등록하려고 했습니다.");
		return;
	}

	AllDialogueWindows[LevelName] = _Window;
}

// 이벤트 구현

void UEventManager::SetLevel(std::string_view _LevelName)
{
	CurLevelName = UEngineString::ToUpper(_LevelName);
	GEngine->UEngineCore::ChangeLevel(_LevelName);
}

void UEventManager::SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point)
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

	// 트리거인 경우 트리거 맵도 수정해준다.
	if (true == AllTriggers[MapName].contains(_Point))
	{
		std::list<AEventTrigger*>::iterator StartIter = AllTriggers[MapName][_Point].begin();
		std::list<AEventTrigger*>::iterator EndIter = AllTriggers[MapName][_Point].end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AEventTrigger* Trigger = *StartIter;

			if (Trigger->GetName() != Target->GetName())
			{
				continue;
			}

			AllTriggers[MapName][_Point].erase(StartIter);
			AllTriggers[MapName][FTileVector(TargetPosition)].push_back(Trigger);
			return;
		}
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
	Target->ChangeMoveAnimation(Target->GetMoveState(), _Direction);
}

void UEventManager::SetCurLevelPlayerState(EPlayerState _State)
{
	APlayer* Player = FindCurLevelTarget<APlayer>(Global::Player);
	Player->StateChange(_State);
}

void UEventManager::FadeChangeLevel(std::string_view _TargetMapName, bool _PlayerControl, float _FadeInTime, float _FadeOutTime)
{
	AFadeLevelChanger* LevelChanger = FindTarget<AFadeLevelChanger>(CurLevelName, Global::FadeLevelChanger);

	if (nullptr == LevelChanger)
	{
		MsgBoxAssert(CurLevelName + "에 FadeLevelChanger가 생성되지 않았습니다.");
		return;
	}

	LevelChanger->Trigger(_TargetMapName, _PlayerControl, _FadeInTime, _FadeOutTime);
}

void UEventManager::WildBattle(const FWildPokemonConstructorParam _ConstructorParam)
{
	AWildBattleTrigger* WildBattleTrigger = FindTarget<AWildBattleTrigger>(CurLevelName, Global::WildBattleTrigger);

	if (nullptr == WildBattleTrigger)
	{
		MsgBoxAssert(std::string(CurLevelName) + "에 WildBattleTrigger가 생성되지 않았습니다.");
		return;
	}

	WildBattleTrigger->Trigger(_ConstructorParam);
}

void UEventManager::OpenMenuWindow()
{
	AMenuCanvas* Canvas = FindCurLevelCommonCanvas<AMenuCanvas>(Global::MenuWindow);

	if (nullptr == Canvas)
	{
		MsgBoxAssert(CurLevelName + "에 메뉴창이 존재하지 않습니다.");
		return;
	}

	APlayer* CurLevelPlayer = FindCurLevelTarget<APlayer>(Global::Player);
	CurLevelPlayer->StateChange(EPlayerState::OutOfControl);
	Canvas->Open();
}

void UEventManager::OpenDialogueWindow(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace)
{
	ADialogueWindow* Window = FindCurLevelDialogueWindow();

	if (nullptr == Window)
	{
		MsgBoxAssert(CurLevelName + "에 메뉴창이 존재하지 않습니다.");
		return;
	}

	APlayer* CurLevelPlayer = FindCurLevelTarget<APlayer>(Global::Player);
	CurLevelPlayer->StateChange(EPlayerState::OutOfControl);
	Window->Open(_Dialogue, _Color, _LineSpace);
}

void UEventManager::ActivatePlayer()
{
	APlayer* CurLevelPlayer = FindCurLevelTarget<APlayer>(Global::Player);
	CurLevelPlayer->StateChange(EPlayerState::Idle);
}

void UEventManager::DeactivatePlayer()
{
	APlayer* CurLevelPlayer = FindCurLevelTarget<APlayer>(Global::Player);
	CurLevelPlayer->StateChange(EPlayerState::OutOfControl);
}

ADialogueWindow* UEventManager::FindCurLevelDialogueWindow()
{
	std::string LevelName = UEngineString::ToUpper(CurLevelName);

	if (false == AllDialogueWindows.contains(LevelName))
	{
		return nullptr;
	}

	ADialogueWindow* Window = AllDialogueWindows[LevelName];
	return Window;
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