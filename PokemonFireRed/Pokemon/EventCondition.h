#pragma once
#include <list>
#include <functional>
#include "PokemonMath.h"


class EventManager;

enum class EEventTriggerAction
{
	Click,  // �÷��̾ ZŰ �Է�
	Notice, // �÷��̾ �پ �ٶ� ���
	StepOn, // �÷��̾ �ش� ��ġ�� �ö� ���
	Direct  // ���� ���� �̺�Ʈ�� ���� �߻���ų ��� 
};

using CheckFunc = std::function<bool(void)>;
#define ToCheckFunc(Func) [this](){return Func();}

// �̺�Ʈ �߻� ������ �����Ѵ�.
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

