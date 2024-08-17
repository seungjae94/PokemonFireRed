#include "EventManager.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/EngineResourcesManager.h>
#include "SoundManager.h"
#include "EventTarget.h"
#include "EventTrigger.h"
#include "EventProcessor.h"
#include "EventStream.h"
#include "PlayerCharacter.h"
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

void UEventManager::RemoveTarget(std::string_view _MapName, std::string_view _TargetName)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	if (true == AllTargets.contains(MapName) && true == AllTargets[MapName].contains(TargetName))
	{
		AllTargets[MapName].erase(TargetName);
	}
}

void UEventManager::RemoveTrigger(std::string_view _MapName, std::string_view _TriggerName)
{
	RemoveTarget(_MapName, _TriggerName);

	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TriggerName = UEngineString::ToUpper(_TriggerName);

	if (false == AllTriggers.contains(MapName))
	{
		return;
	}

	for (std::pair<const FTileVector, std::list<AEventTrigger*>>& Pair : AllTriggers.at(MapName))
	{
		std::list<AEventTrigger*>& TriggerList = Pair.second;

		std::list<AEventTrigger*>::iterator StartIter = TriggerList.begin();
		std::list<AEventTrigger*>::iterator EndIter = TriggerList.end();

		for (; StartIter != EndIter;)
		{
			AEventTrigger* Trigger = *StartIter;

			if (nullptr == Trigger)
			{
				MsgBoxAssert("UEventManager::AllTriggers[" + MapName + "][" + Pair.first.ToString() + "]�� nullptr�� Ʈ���Ű� ����Ǿ� �ֽ��ϴ�.");
				return;
			}

			if (UEngineString::ToUpper(Trigger->GetName()) == TriggerName)
			{
				StartIter = TriggerList.erase(StartIter);
				continue;
			}

			++StartIter;
		}
	}
}

// �̺�Ʈ ����
void UEventManager::Tick(float _DeltaTime)
{
	DeltaTime = _DeltaTime;

	USoundManager::Tick(_DeltaTime);

	if (true == UEventProcessor::IsRunning())
	{
		UEventProcessor::Tick(_DeltaTime);
	}
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
		MsgBoxAssert("nullptr�� �̺�Ʈ Ÿ������ �߰��� �� �����ϴ�.");
		return;
	}

	std::string TargetName = _Setting.Name;
	std::string ImageName = _Setting.ImageName;
	std::string AnimNamePrefix = _Setting.AnimNamePrefix;
	std::string LevelName = _Target->GetWorld()->GetName();

	// ��� ���� �ʱ�ȭ
	_Target->SetName(TargetName);

	_Target->Point = _Setting.Point;						// ���� ������� ���� Ÿ���̶� SetPoint�� ��ġ ������ �Ұ����ϴ�.
	_Target->SetActorLocation(_Setting.Point.ToFVector());	// ���� ������� ���� Ÿ���̶� SetPoint�� ��ġ ������ �Ұ����ϴ�.

	_Target->Collidable = _Setting.Collidable;
	_Target->Rotatable = _Setting.Rotatable;
	_Target->Walkable = _Setting.Walkable;
	_Target->Direction = _Setting.Direction;
	_Target->ImageName = _Setting.ImageName;
	_Target->AnimNamePrefix = _Setting.AnimNamePrefix;
	_Target->Height = _Setting.Height;

	if (_Target->ImageName != "")
	{
		_Target->HasImage = true;
	}

	if (AnimNamePrefix == "")
	{
		AnimNamePrefix = TargetName;
	}

	if (true == AllTargets[LevelName].contains(TargetName))
	{
		MsgBoxAssert("�̹� ��ϵ� �̺�Ʈ Ÿ�� " + TargetName + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllTargets[LevelName][TargetName] = _Target;

	// ������ �ʱ�ȭ
	if (true == _Target->HasImage)
	{
		UWindowImage* Image = UEngineResourcesManager::GetInst().FindImg(ImageName);
		if (nullptr == Image)
		{
			MsgBoxAssert(ImageName + "�� �������� �ʴ� �̹����Դϴ�. �̺�Ʈ Ÿ�� " + TargetName + " ������ �����߽��ϴ�.");
			return;
		}

		UImageRenderer* LowerBodyRenderer = nullptr;
		UImageRenderer* UpperBodyRenderer = nullptr;

		_Target->LowerBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Lower);
		LowerBodyRenderer = _Target->LowerBodyRenderer;
		LowerBodyRenderer->SetImage(ImageName);
		LowerBodyRenderer->SetTransform({ {0, 0}, {Global::TileSize, Global::TileSize} });

		// ���Ʒ� 2ĭ �̹����� ��쿡�� UpperBody�� �������Ѵ�.
		if (_Target->Height == 2)
		{
			_Target->UpperBodyRenderer = _Target->CreateImageRenderer(ERenderingOrder::Upper);
			UpperBodyRenderer = _Target->UpperBodyRenderer;
			UpperBodyRenderer->SetImage(ImageName);
			UpperBodyRenderer->SetTransform({ {0, -Global::TileSize}, {Global::TileSize, Global::TileSize} });
		}

		// �ִϸ��̼� ����
		std::vector<std::string> AllDirectionNames = FTileVector::AllDirectionNames();

		if (true == _Target->Rotatable)
		{
			int FrameIndex = 0;
			for (std::string& DirectionName : AllDirectionNames)
			{
				std::string ImageName = AnimNamePrefix + "Idle.png";

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
				std::string ImageName = AnimNamePrefix + "Walk" + DirectionName + ".png";

				std::string UpperBodyAnimName = TargetName + "Walk" + DirectionName + Global::SuffixUpperBody;
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "0", ImageName, { 0, 1 }, WalkInterval, false); // ������
				UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "1", ImageName, { 2, 3 }, WalkInterval, false); // �޹�

				std::string LowerBodyAnimName = TargetName + "Walk" + DirectionName + Global::SuffixLowerBody;
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "0", ImageName, { 4, 5 }, WalkInterval, false); // ������
				LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "1", ImageName, { 6, 7 }, WalkInterval, false); // �޹�
			}
		}
	}

	// �ݸ��� �ʱ�ȭ
	if (true == _Target->Collidable)
	{
		ECollisionOrder Order = ECollisionOrder::NPC;

		if (nullptr != dynamic_cast<APlayerCharacter*>(_Target))
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
}

void UEventManager::AddPlayer(APlayerCharacter* _Player, const FTileVector& _Point)
{
	UEventTargetSetting PlayerSetting;
	PlayerSetting.SetName(Global::PlayerCharacter);
	PlayerSetting.SetPoint(_Point);
	PlayerSetting.SetDirection(FTileVector::Down);
	PlayerSetting.SetCollidable(true);
	PlayerSetting.SetRotatable(true);
	PlayerSetting.SetWalkable(true);
	PlayerSetting.SetImageNameAuto();

	AddTarget(_Player, PlayerSetting);

	// �÷��̾� ������ ��� �ʱ�ȭ
	_Player->StateChange(EPlayerState::Idle);

	// �÷��̾� �ִϸ��̼� ����
	std::vector<std::string> AllDirectionNames = FTileVector::AllDirectionNames();

	// �ִϸ��̼� - ����
	float JumpInterval = Global::CharacterJumpAnimFrameLength;
	_Player->UpperBodyRenderer->CreateAnimation("PlayerCharacterJumpDown" + Global::SuffixUpperBody,
		"PlayerCharacterJumpDown.png", 0, 52, JumpInterval, false);
	_Player->LowerBodyRenderer->CreateAnimation("PlayerCharacterJumpDown" + Global::SuffixLowerBody,
		"PlayerCharacterJumpDown.png", 53 + 0, 53 + 52, JumpInterval, false);

	// �ִϸ��̼� - ������ �ȱ�
	float WalkInterval = 1.0f / Global::CharacterWalkSpeed / 2;
	float SlowWalkInterval = WalkInterval * 2.0f;

	for (std::string& DirectionName : AllDirectionNames)
	{
		std::string ImageName = Global::PlayerCharacter + "Walk" + DirectionName + ".png";

		std::string UpperBodyAnimName = Global::PlayerCharacter + "SlowWalk" + DirectionName + Global::SuffixUpperBody;
		_Player->UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "0", ImageName, { 0, 1 }, SlowWalkInterval, false); // ������
		_Player->UpperBodyRenderer->CreateAnimation(UpperBodyAnimName + "1", ImageName, { 2, 3 }, SlowWalkInterval, false); // �޹�

		std::string LowerBodyAnimName = Global::PlayerCharacter + "SlowWalk" + DirectionName + Global::SuffixLowerBody;
		_Player->LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "0", ImageName, { 4, 5 }, SlowWalkInterval, false); // ������
		_Player->LowerBodyRenderer->CreateAnimation(LowerBodyAnimName + "1", ImageName, { 6, 7 }, SlowWalkInterval, false); // �޹�
	}
}

void UEventManager::AddCommonCanvas(ACanvas* _UIElement, std::string_view _Name)
{
	std::string LevelName = UEngineString::ToUpper(_UIElement->GetWorld()->GetName());
	std::string Name = UEngineString::ToUpper(_Name.data());

	if (true == AllCommonCanvas[LevelName].contains(Name))
	{
		MsgBoxAssert("�̹� ��ϵ� UI ������Ʈ" + Name + "�� �ٽ� ����Ϸ��� �߽��ϴ�.");
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
		MsgBoxAssert("�̹� ��ϵ� ��ȭâ�� �ٽ� ����Ϸ��� �߽��ϴ�.");
		return;
	}

	AllDialogueWindows[LevelName] = _Window;
}

// �̺�Ʈ ����

void UEventManager::ChangeArea(std::string_view _AreaName, std::string_view _AreaBgm)
{
	APlayerCharacter* Player = FindCurLevelTarget<APlayerCharacter>(EN::PlayerCharacter);

	if (nullptr == Player)
	{
		MsgBoxAssert("�� ������ �ƴѵ� ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}

	UMapLevel* MapLevel = dynamic_cast<UMapLevel*>(Player->GetWorld());

	if (nullptr == MapLevel)
	{
		MsgBoxAssert("�� ������ �ƴѵ� ������ �����Ϸ��� �߽��ϴ�.");
		return;
	}

	MapLevel->SetAreaName(_AreaName);
	MapLevel->SetAreaBgm(_AreaBgm);
}

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
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� �̵���Ű���� �߽��ϴ�.");
		return;
	}

	FTileVector PrevPoint = Target->Point;

	FVector TargetPosition = _Point.ToFVector();
	Target->SetActorLocation(TargetPosition);
	Target->Point = _Point;

	// Ʈ������ ��� Ʈ���� �ʵ� �������ش�.
	if (true == AllTriggers[MapName].contains(PrevPoint))
	{
		std::list<AEventTrigger*>::iterator StartIter = AllTriggers[MapName][PrevPoint].begin();
		std::list<AEventTrigger*>::iterator EndIter = AllTriggers[MapName][PrevPoint].end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AEventTrigger* Trigger = *StartIter;

			if (Trigger->GetName() != Target->GetName())
			{
				continue;
			}

			AllTriggers[MapName][PrevPoint].erase(StartIter);
			AllTriggers[MapName][_Point].push_back(Trigger);
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
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� ȸ����Ű���� �߽��ϴ�.");
		return;
	}

	Target->SetDirection(_Direction);
	Target->ChangeMoveAnimation(Target->GetMoveState(), _Direction);
}

void UEventManager::SetActive(std::string_view _MapName, std::string_view _TargetName, bool _Value)
{
	std::string MapName = UEngineString::ToUpper(_MapName);
	std::string TargetName = UEngineString::ToUpper(_TargetName);

	AEventTarget* Target = AllTargets[MapName][TargetName];

	if (nullptr == Target)
	{
		MsgBoxAssert("�������� �ʴ� Ÿ��" + MapName + ":" + TargetName + "�� ȸ����Ű���� �߽��ϴ�.");
		return;
	}

	Target->SetActive(_Value);
}

void UEventManager::SetCurLevelPlayerState(EPlayerState _State)
{
	APlayerCharacter* Player = FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	Player->StateChange(_State);
}

void UEventManager::FadeChangeLevel(std::string_view _TargetMapName, bool _PlayerControl, float _FadeInTime, float _FadeOutTime, bool _IsFadeBgm, std::string_view _NewBgm)
{
	if (true == UPlayerData::IsAchieved(EAchievement::Fading))
	{
		return;
	}

	AFadeLevelChanger* LevelChanger = FindTarget<AFadeLevelChanger>(CurLevelName, Global::FadeLevelChanger);

	if (nullptr == LevelChanger)
	{
		MsgBoxAssert(CurLevelName + "�� FadeLevelChanger�� �������� �ʾҽ��ϴ�.");
		return;
	}

	LevelChanger->Trigger(_TargetMapName, _PlayerControl, _FadeInTime, _FadeOutTime, _IsFadeBgm, _NewBgm);
}

void UEventManager::WildBattle(const FWildPokemonConstructorParam _ConstructorParam)
{
	AWildBattleTrigger* WildBattleTrigger = FindTarget<AWildBattleTrigger>(CurLevelName, Global::WildBattleTrigger);

	if (nullptr == WildBattleTrigger)
	{
		MsgBoxAssert(std::string(CurLevelName) + "�� WildBattleTrigger�� �������� �ʾҽ��ϴ�.");
		return;
	}

	WildBattleTrigger->Trigger(_ConstructorParam);
}

void UEventManager::OpenMenuWindow()
{
	AMenuCanvas* Canvas = FindCurLevelCommonCanvas<AMenuCanvas>(Global::MenuWindow);

	if (nullptr == Canvas)
	{
		MsgBoxAssert(CurLevelName + "�� �޴�â�� �������� �ʽ��ϴ�.");
		return;
	}

	APlayerCharacter* CurLevelPlayer = FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	CurLevelPlayer->StateChange(EPlayerState::OutOfControl);
	USoundManager::PlaySE(RN::SEMenu);
	Canvas->Open();
}

void UEventManager::OpenDialogueWindow(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace)
{
	ADialogueWindow* Window = FindCurLevelDialogueWindow();

	if (nullptr == Window)
	{
		MsgBoxAssert(CurLevelName + "�� �޴�â�� �������� �ʽ��ϴ�.");
		return;
	}

	APlayerCharacter* CurLevelPlayer = FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	CurLevelPlayer->StateChange(EPlayerState::OutOfControl);
	Window->Open(_Dialogue, _Color, _LineSpace);
}

void UEventManager::ActivatePlayer()
{
	APlayerCharacter* CurLevelPlayer = FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
	CurLevelPlayer->StateChange(EPlayerState::Idle);
}

void UEventManager::DeactivatePlayer()
{
	APlayerCharacter* CurLevelPlayer = FindCurLevelTarget<APlayerCharacter>(Global::PlayerCharacter);
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