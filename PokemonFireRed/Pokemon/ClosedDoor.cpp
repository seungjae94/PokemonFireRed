#include "ClosedDoor.h"

AClosedDoor::AClosedDoor() 
{
}

AClosedDoor::~AClosedDoor() 
{
}

void AClosedDoor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond = UEventCondition(EEventTriggerAction::ZClick);

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::Chat({ L"The door is firmly closed." }, EFontColor::Gray, 16)
		>> ES::End(true)
	);
}

