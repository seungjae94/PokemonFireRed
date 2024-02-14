#include "EventCondition.h"

UEventCondition::UEventCondition(EEventTriggerAction _TriggerAction)
{
	TriggerAction = _TriggerAction;
}

UEventCondition::UEventCondition()
{

}

UEventCondition::~UEventCondition()
{
}

bool UEventCondition::Check(EEventTriggerAction _TriggerAction) const
{
	if (TriggerAction != _TriggerAction)
	{
		return false;
	}

	for (CheckFunc Func : AllCheckFunctions)
	{
		if (false == Func())
		{
			return false;
		}
	}

	return true;
}
