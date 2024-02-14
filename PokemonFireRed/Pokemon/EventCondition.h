#pragma once
#include <list>
#include <functional>
#include "PokemonMath.h"


class EventManager;

enum class EEventTriggerAction
{
	Click,
	Notice,
	StepOn
};

using CheckFunc = std::function<bool(void)>;

// 이벤트 발생 조건을 정의한다.
class UEventCondition
{
public:
	// constructor destructor
	UEventCondition();
	UEventCondition(EEventTriggerAction _TriggerAction);
	~UEventCondition();

	// delete Function
	//UEventCondition(const UEventCondition& _Other) = delete;
	//UEventCondition(UEventCondition&& _Other) noexcept = delete;
	//UEventCondition& operator=(const UEventCondition& _Other);
	//UEventCondition& operator=(UEventCondition&& _Other) noexcept = delete;

	void RegisterCheckFunc(CheckFunc _Func)
	{
		AllCheckFunctions.push_back(_Func);
	}

	/// <summary>
	/// 트리거 액션의 일치 여부와 플레이어의 상태 등을 확인해 이벤트 발생 조건 충족 여부를 계산해서 반환한다.
	/// </summary>
	bool Check(EEventTriggerAction _TriggerAction) const;

	bool operator<(const UEventCondition& _Other) const
	{
		return TriggerAction < _Other.TriggerAction;
	}

protected:

private:
	EEventTriggerAction TriggerAction = EEventTriggerAction::Click;
	std::list<CheckFunc> AllCheckFunctions;
};

