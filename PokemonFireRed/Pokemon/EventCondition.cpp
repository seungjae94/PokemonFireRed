#include "EventCondition.h"

int UEventCondition::IdGenerator = 0;

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
