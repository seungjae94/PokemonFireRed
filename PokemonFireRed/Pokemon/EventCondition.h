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

// �̺�Ʈ �߻� ������ �����Ѵ�.
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
	/// Ʈ���� �׼��� ��ġ ���ο� �÷��̾��� ���� ���� Ȯ���� �̺�Ʈ �߻� ���� ���� ���θ� ����ؼ� ��ȯ�Ѵ�.
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

