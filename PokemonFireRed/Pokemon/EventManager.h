#pragma once
#include <map>
#include <vector>
#include <functional>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include "PokemonMath.h"
#include "EventCondition.h"

class UPokemonLevel;
class AEventTarget;
class UEventTargetInit;
class AEventTrigger;
class UEventProcessor;
class UEventStream;
class APlayer;
class UEventManagerReleaser;
class UEventCondition;
class AUIElement;
enum class EFontColor;

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

	// ��� �̺�Ʈ �Լ�
	// - ���̵� ����Ʈ�� ���� �̺�Ʈ ����� 
	//   �̺�Ʈ Ʈ���Ÿ� ������ �ʰ� ��� �����ص� ������ ���� �ʴ´�.

	static void SetLevel(std::string_view _LevelName);

	static void SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static void SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);
	
	// ã�� ���� �Լ�
	template <typename UIType>
	static UIType* FindUIElement(std::string_view _LevelName, std::string_view _ElementName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string ElementName = UEngineString::ToUpper(_ElementName);

		if (false == AllUIElements.contains(LevelName) || false == AllUIElements[LevelName].contains(ElementName))
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "�� �������� �ʴ� UI ������Ʈ�Դϴ�. FindUIElement ���࿡ �����߽��ϴ�.");
			return nullptr;
		}

		AUIElement* Element = AllUIElements[LevelName][ElementName];
		UIType* Result = dynamic_cast<UIType*>(Element);
		
		if (nullptr == Result)
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "�� �ٿ� ĳ���ÿ� �����߽��ϴ�. FindUIElement ���࿡ �����߽��ϴ�.");
			return nullptr;
		}

		return Result;
	}
	
	template <typename UIType>
	static UIType* FindCurLevelUIElement(std::string_view _TargetName)
	{
		return FindUIElement<UIType>(CurLevelName, _TargetName);
	}

	template <typename TargetType>
	static TargetType* FindTarget(std::string_view _LevelName, std::string_view _TargetName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string TargetName = UEngineString::ToUpper(_TargetName);

		if (false == AllTargets.contains(LevelName) || false == AllTargets[LevelName].contains(TargetName))
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "�� �������� �ʴ� �̺�Ʈ Ÿ���Դϴ�. FindTarget ���࿡ �����߽��ϴ�.");
			return nullptr;
		}

		AEventTarget* Target = AllTargets[LevelName][TargetName];
		TargetType* Result = dynamic_cast<TargetType*>(Target);

		if (nullptr == Result)
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "�� �ٿ� ĳ���ÿ� �����߽��ϴ�. FindTarget ���࿡ �����߽��ϴ�.");
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

	// AllMenuWindows[LevelName][ElementName]
	// - �޴�â, ��ȭâ�� Ŀ�� �̵� �� �÷��̾� Ÿ������ �ٷ�� �� ���� �ֱ� ������ �߰��� �����Ѵ�.
	static std::map<std::string, std::map<std::string, AUIElement*>> AllUIElements;

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
	
	static void Tick(float _DeltaTime);
	
	static void AddTarget(AEventTarget* _Target, const UEventTargetInit& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetInit& _Setting);
	static void AddPlayer(APlayer* _Player, const FTileVector& _Point);
	static void AddUIElement(AUIElement* _UIElement, std::string_view _Name);

	// DeltaTime ���
	static float DeltaTime;

	// �̺�Ʈ ���μ��� ������
	static void Release();
};