#pragma once
#include <list>
#include <functional>
#include "PokemonMath.h"

class UEventManager;

enum class EEventTriggerAction
{
	ZClick,		// 플레이어가 트리거에 인접 and 트리거를 바라봄 and Z키 입력 (NPC 등)
	Read,		// 플레이어가 트리거에 인접 and 트리거를 바라봄 (표지판 등)
	ArrowClick,	// 플레이어가 트리거에 인접 and 트리거가 있는 방향의 방향키 입력 (문, 계단 등)
	StepOn,		// 플레이어가 트리거와 같은 위치에 있음 (태초마을 처음 나갈 때 이벤트를 발생시키는 발판 등)
	Direct		// 이벤트를 직접 발생시키는 경우 (타이틀 레벨 페이드 아웃 트리거 등)
};

using CheckFunc = std::function<bool(void)>;
#define ToCheckFunc(Func) [this](){return Func();}

// 이벤트 발생 조건을 정의한다.
class UEventCondition
{
public:
	// constructor destructor
	UEventCondition()
		: TriggerAction(EEventTriggerAction::Direct)
	{
	}

	UEventCondition(EEventTriggerAction _TriggerAction)
		: TriggerAction(_TriggerAction)
	{
	}

	~UEventCondition();

	void RegisterCheckFunc(CheckFunc _Func)
	{
		AllCheckFunctions.push_back(_Func);
	}

	/// <summary>
	/// 트리거 액션의 일치 여부와 플레이어의 상태 등을 확인해 이벤트 발생 조건 충족 여부를 계산해서 반환한다.
	/// </summary>
	bool Check(EEventTriggerAction _TriggerAction) const;

	// 주소 비교
	bool operator<(const UEventCondition& _Other) const
	{
		return reinterpret_cast<__int64>(this) < reinterpret_cast<__int64>(&_Other);
	}

protected:

private:
	EEventTriggerAction TriggerAction = EEventTriggerAction::ZClick;
	std::list<CheckFunc> AllCheckFunctions;
};

