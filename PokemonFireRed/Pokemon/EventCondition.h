#pragma once
#include <list>
#include <functional>
#include "PokemonMath.h"

class UEventManager;

enum class EEventTriggerAction
{
	ZClick,		// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ� and ZŰ �Է� (NPC ��)
	Read,		// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ÿ� �ٶ� (ǥ���� ��)
	ArrowClick,	// �÷��̾ Ʈ���ſ� ���� and Ʈ���Ű� �ִ� ������ ����Ű �Է� (��, ��� ��)
	StepOn,		// �÷��̾ Ʈ���ſ� ���� ��ġ�� ���� (���ʸ��� ó�� ���� �� �̺�Ʈ�� �߻���Ű�� ���� ��)
	Direct		// �̺�Ʈ�� ���� �߻���Ű�� ��� (Ÿ��Ʋ ���� ���̵� �ƿ� Ʈ���� ��)
};

using CheckFunc = std::function<bool(void)>;
#define ToCheckFunc(Func) [this](){return Func();}

// �̺�Ʈ �߻� ������ �����Ѵ�.
class UEventCondition
{
public:
	// constructor destructor
	UEventCondition()
		: TriggerAction(EEventTriggerAction::Direct)
	{
		Id = ++IdGenerator;
	}

	UEventCondition(EEventTriggerAction _TriggerAction)
		: TriggerAction(_TriggerAction)
	{
		Id = ++IdGenerator;
	}

	~UEventCondition();

	void RegisterCheckFunc(CheckFunc _Func)
	{
		AllCheckFunctions.push_back(_Func);
	}

	/// <summary>
	/// Ʈ���� �׼��� ��ġ ���ο� �÷��̾��� ���� ���� Ȯ���� �̺�Ʈ �߻� ���� ���� ���θ� ����ؼ� ��ȯ�Ѵ�.
	/// </summary>
	bool Check(EEventTriggerAction _TriggerAction) const;

	bool operator==(const UEventCondition& _Other) const
	{
		return Id == _Other.Id;
	}

	bool operator<(const UEventCondition& _Other) const
	{
		return Id < _Other.Id;
	}

protected:

private:
	int Id = 0;
	static int IdGenerator;
	EEventTriggerAction TriggerAction = EEventTriggerAction::ZClick;
	std::list<CheckFunc> AllCheckFunctions;
};

