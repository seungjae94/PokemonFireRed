#pragma once
#include <list>
#include <functional>
#include "PokemonMath.h"


class EventManager;

enum class EEventTriggerAction
{
	Click,  // 플레이어가 Z키 입력
	Notice, // 플레이어가 붙어서 바라볼 경우
	StepOn, // 플레이어가 해당 위치에 올라갈 경우
	Direct  // 레벨 등이 이벤트를 직접 발생시킬 경우 
};

using CheckFunc = std::function<bool(void)>;
#define ToCheckFunc(Func) [this](){return Func();}

// 이벤트 발생 조건을 정의한다.
class UEventCondition
{
public:
	// constructor destructor
	UEventCondition();
	UEventCondition(EEventTriggerAction _TriggerAction);
	~UEventCondition();

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

