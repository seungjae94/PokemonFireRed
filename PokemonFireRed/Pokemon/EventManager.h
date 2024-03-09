#pragma once
#include <map>
#include <vector>
#include <functional>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include <EngineCore/Level.h>
#include "PokemonMath.h"
#include "EventCondition.h"
#include "Pokemon.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetSetting;
class AEventTrigger;
class UEventProcessor;
class UEventStream;
class APlayer;
class UEventManagerReleaser;
class UEventCondition;
class ACanvas;
class ADialogueWindow;
class ATrainer;
enum class EFontColor;
enum class EPlayerState;

// ��� �̺�Ʈ ó���� ����ϴ� Ŭ����
// - �� �������� ���͸� �����Ϸ��� �ݵ�� EventManager Ŭ������ ���ؼ� �����ؾ� �Ѵ�.
class UEventManager
{
	friend UEventProcessor;
	friend UPokemonLevel;
	friend UEventManagerReleaser;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	// �̺�Ʈ ���
	static void RegisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition, UEventStream _Stream);
	static void UnregisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition);

	// �̺�Ʈ Ʈ����
	static bool TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action = EEventTriggerAction::Direct);

	// ��Ʋ Enemy ������ ���̺� �ε�
	static void SaveEnemyEntry(std::vector<UPokemon>* _Entry);
	static std::vector<UPokemon>* LoadEnemyEntry();
	static bool IsWildPokemonBattle();
	static void SetAsWildPokemonBattle();
	static void SetAsTrainerBattle(ATrainer* _Trainer);
	static ATrainer* GetTrainer()
	{
		return Trainer;
	}


	// ��� �̺�Ʈ �Լ�
	// - ���̵� ����Ʈ�� ���� �̺�Ʈ ����� 
	//   �̺�Ʈ Ʈ���Ÿ� ������ �ʰ� ��� �����ص� ������ ���� �ʴ´�.

	static void SetLevel(std::string_view _LevelName);

	static void SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static void SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);
	
	static void SetCurLevelPlayerState(EPlayerState _State);

	static void FadeChangeLevel(std::string_view _TargetLevelName, bool _PlayerControl = true, float _FadeInTime = 0.3f, float _FadeOutTime = 0.3f);
	
	static void WildBattle(const FWildPokemonConstructorParam _ConstructorParam);

	static void OpenMenuWindow();

	static void OpenDialogueWindow(const std::vector<std::wstring>& _Dialogue, EFontColor _Color, int _LineSpace = 14);

	static void ActivatePlayer();

	static void DeactivatePlayer();

	// ã�� ���� �Լ�
	static ADialogueWindow* FindCurLevelDialogueWindow();

	template <typename CanvasType>
	static CanvasType* FindCommonCanvas(std::string_view _LevelName, std::string_view _CanvasName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string CanvasName = UEngineString::ToUpper(_CanvasName);

		if (false == AllCommonCanvas.contains(LevelName) || false == AllCommonCanvas[LevelName].contains(CanvasName))
		{
			return nullptr;
		}

		ACanvas* Element = AllCommonCanvas[LevelName][CanvasName];
		CanvasType* Result = dynamic_cast<CanvasType*>(Element);
		
		if (nullptr == Result)
		{
			return nullptr;
		}

		return Result;
	}
	
	template <typename CanvasType>
	static CanvasType* FindCurLevelCommonCanvas(std::string_view _CanvasName)
	{
		return FindCommonCanvas<CanvasType>(CurLevelName, _CanvasName);
	}

	template <typename TargetType>
	static TargetType* FindTarget(std::string_view _LevelName, std::string_view _TargetName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string TargetName = UEngineString::ToUpper(_TargetName);

		if (false == AllTargets.contains(LevelName) || false == AllTargets[LevelName].contains(TargetName))
		{
			return nullptr;
		}

		AEventTarget* Target = AllTargets[LevelName][TargetName];
		TargetType* Result = dynamic_cast<TargetType*>(Target);

		if (nullptr == Result)
		{
			return nullptr;
		}

		return Result;
	}
	
	template <typename TargetType>
	static TargetType* FindCurLevelTarget(std::string_view _TargetName)
	{
		return FindTarget<TargetType>(CurLevelName, _TargetName);
	}

	// �־��� ��ġ�� ��� Ʈ���Ÿ� ��ȯ
	template <typename TriggerType>
	static std::list<TriggerType*> FindTriggerListAt(std::string_view _LevelName, const FTileVector& _Point)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);

		if (false == AllTriggers.contains(LevelName) || false == AllTriggers[LevelName].contains(_Point))
		{
			return {};
		}

		std::list<AEventTrigger*> Triggers = AllTriggers[LevelName][_Point];
		std::list<TriggerType*> CastedTriggers;

		for (AEventTrigger* Trigger : Triggers)
		{
			TriggerType* Result = dynamic_cast<TriggerType*>(Trigger);

			if (nullptr != Result)
			{
				CastedTriggers.push_back(Result);
			}
		}
		
		return CastedTriggers;
	}

	template <typename TriggerType>
	static std::list<TriggerType*> FindCurLevelTriggerListAt(const FTileVector& _Point)
	{
		return FindTriggerListAt<TriggerType>(CurLevelName, _Point);
	}

	// �־��� ��ġ�� Ʈ���Ÿ� �ϳ��� ã�Ƽ� ��ȯ
	template <typename TriggerType>
	static TriggerType* FindTriggerAt(std::string_view _LevelName, const FTileVector& _Point)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);

		if (false == AllTriggers.contains(LevelName) || false == AllTriggers[LevelName].contains(_Point))
		{
			return {};
		}

		std::list<AEventTrigger*> Triggers = AllTriggers[LevelName][_Point];

		for (AEventTrigger* Trigger : Triggers)
		{
			TriggerType* Result = dynamic_cast<TriggerType*>(Trigger);

			if (nullptr != Result)
			{
				return Result;
			}
		}

		return nullptr;
	}

	template <typename TriggerType>
	static TriggerType* FindCurLevelTriggerAt(const FTileVector& _Point)
	{
		return FindTriggerAt<TriggerType>(CurLevelName, _Point);
	}

	static std::string GetCurLevelName()
	{
		return CurLevelName;
	}

	static float GetDeltaTime()
	{
		return DeltaTime;
	}

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	static std::string CurLevelName;
	static ULevel* BattleLevel;

	// AllMapLevelCanvas[LevelName][ElementName]
	// - �޴�â�� Ŀ�� �̵� �� �÷��̾� Ÿ������ �ٷ�� �� ���� �ֱ� ������ �߰��� �����Ѵ�.
	static std::map<std::string, std::map<std::string, ACanvas*>> AllCommonCanvas;

	static std::map<std::string, ADialogueWindow*> AllDialogueWindows;

	// AllTargets[LevelName][TargetName]
	// - 'A������ B��� �̸��� ���͸� �̵�������'��� ��û�� ó���Ϸ��� A������ B��� �̸��� ���͸� ã�� �� �־�� �Ѵ�.
	static std::map<std::string, std::map<std::string, AEventTarget*>> AllTargets;

	// AllTriggers[LevelName][Point] = list of Triggers
	// - �÷��̾ Ư�� ��ġ�� Ʈ���Ű� �ִ��� Ȯ���� �� ���
	// - �ش� Ʈ���Ÿ� ���۽�ų �� ���
	static std::map<std::string, std::map<FTileVector, std::list<AEventTrigger*>>> AllTriggers;

	// AllProcessors[Trigger]
	// - �̺�Ʈ �Ŵ����� ���� ����� �����ϰ� ��� ������ �� ������ ��� Ʈ������ �ݹ�(�̺�Ʈ)�� �����ϱ⿡ ���������� �ʴ�.
	// - Ʈ���ſ� �ϴ��Ϸ� �����ؼ� �ݹ��� ����ϰ� ���� ����� �����ϰ� �����ϸ鼭 �������ִ� ��ü�� �ʿ��ϴ�.
	// - �̺�Ʈ ���μ����� �ٷ� �� ������ �ð� �ȴ�.
	static std::map<AEventTrigger*, UEventProcessor*> AllProcessors;
	
	// ��Ʋ ���� ����
	static bool IsWildPokemon;
	static std::vector<UPokemon>* EnemyEntry;
	static ATrainer* Trainer;

	// ƽ
	static void Tick(float _DeltaTime);
	
	// ��� �Լ�
	static void AddTarget(AEventTarget* _Target, const UEventTargetSetting& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetSetting& _Setting);
	static void AddPlayer(APlayer* _Player, const FTileVector& _Point);
	static void AddCommonCanvas(ACanvas* _Canvas, std::string_view _Name);
	static void AddDialogueWindow(ADialogueWindow* _Window);

	// DeltaTime ���
	static float DeltaTime;

	// �̺�Ʈ ���μ��� ������
	static void Release();
};