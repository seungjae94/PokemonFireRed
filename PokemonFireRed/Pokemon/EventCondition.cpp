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
	return TriggerAction == _TriggerAction;
}
