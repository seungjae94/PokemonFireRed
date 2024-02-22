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

// 모든 이벤트 처리를 담당하는 클래스
// - 맵 레벨에서 액터를 조작하려면 반드시 EventManager 클래스를 통해서 조작해야 한다.
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

	// 이벤트 등록
	static void RegisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition, UEventStream _Stream);
	static void UnregisterEvent(AEventTrigger* _Trigger, const UEventCondition& _Condition);

	// 이벤트 트리거
	static bool TriggerEvent(AEventTrigger* _Trigger, EEventTriggerAction _Action = EEventTriggerAction::Direct);

	// 즉발 이벤트 함수
	// - 사이드 이펙트가 없는 이벤트 명령은 
	//   이벤트 트리거를 만들지 않고 즉시 실행해도 문제가 되지 않는다.

	static void SetLevel(std::string_view _LevelName);

	static void SetPoint(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Point);

	static void SetDirection(std::string_view _MapName, std::string_view _TargetName, const FTileVector& _Direction);
	
	// 찾기 편의 함수
	template <typename UIType>
	static UIType* FindUIElement(std::string_view _LevelName, std::string_view _ElementName)
	{
		std::string LevelName = UEngineString::ToUpper(_LevelName);
		std::string ElementName = UEngineString::ToUpper(_ElementName);

		if (false == AllUIElements.contains(LevelName) || false == AllUIElements[LevelName].contains(ElementName))
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "은 존재하지 않는 UI 엘리먼트입니다. FindUIElement 실행에 실패했습니다.");
			return nullptr;
		}

		AUIElement* Element = AllUIElements[LevelName][ElementName];
		UIType* Result = dynamic_cast<UIType*>(Element);
		
		if (nullptr == Result)
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "의 다운 캐스팅에 실패했습니다. FindUIElement 실행에 실패했습니다.");
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
			//MsgBoxAssert(LevelName + ":" + TargetName + "은 존재하지 않는 이벤트 타겟입니다. FindTarget 실행에 실패했습니다.");
			return nullptr;
		}

		AEventTarget* Target = AllTargets[LevelName][TargetName];
		TargetType* Result = dynamic_cast<TargetType*>(Target);

		if (nullptr == Result)
		{
			//MsgBoxAssert(LevelName + ":" + TargetName + "의 다운 캐스팅에 실패했습니다. FindTarget 실행에 실패했습니다.");
			return nullptr;
		}

		return Result;
	}
	
	template <typename TargetType>
	static TargetType* FindCurLevelTarget(std::string_view _TargetName)
	{
		return FindTarget<TargetType>(CurLevelName, _TargetName);
	}

	// 주어진 위치의 모든 트리거를 반환
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

	// 주어진 위치의 트리거를 하나만 찾아서 반환
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
	// - 메뉴창, 대화창도 커서 이동 등 플레이어 타입으로 다뤄야 할 일이 있기 때문에 추가로 보관한다.
	static std::map<std::string, std::map<std::string, AUIElement*>> AllUIElements;

	// AllTargets[LevelName][TargetName]
	// - 'A레벨의 B라는 이름의 액터를 이동시켜줘'라는 요청을 처리하려면 A레벨의 B라는 이름의 액터를 찾을 수 있어야 한다.
	static std::map<std::string, std::map<std::string, AEventTarget*>> AllTargets;

	// AllTriggers[LevelName][Point] = list of Triggers
	// - 플레이어가 특정 위치에 트리거가 있는지 확인할 때 사용
	// - 해당 트리거를 동작시킬 때 사용
	static std::map<std::string, std::map<FTileVector, std::list<AEventTrigger*>>> AllTriggers;

	// AllProcessors[Trigger]
	// - 이벤트 매니저는 레벨 변경과 무관하게 계속 존재할 수 있지만 모든 트리거의 콜백(이벤트)을 저장하기에 적합하지는 않다.
	// - 트리거와 일대일로 대응해서 콜백을 기억하고 레벨 변경과 무관하게 존재하면서 실행해주는 객체가 필요하다.
	// - 이벤트 프로세서가 바로 이 역할을 맡게 된다.
	static std::map<AEventTrigger*, UEventProcessor*> AllProcessors;
	
	static void Tick(float _DeltaTime);
	
	static void AddTarget(AEventTarget* _Target, const UEventTargetInit& _Setting);
	static void AddTrigger(AEventTrigger* _Trigger, const UEventTargetInit& _Setting);
	static void AddPlayer(APlayer* _Player, const FTileVector& _Point);
	static void AddUIElement(AUIElement* _UIElement, std::string_view _Name);

	// DeltaTime 기록
	static float DeltaTime;

	// 이벤트 프로세서 릴리즈
	static void Release();
};