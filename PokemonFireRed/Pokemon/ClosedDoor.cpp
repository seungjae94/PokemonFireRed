#include "ClosedDoor.h"
#include "EventMacros.h"
#include "EventStream.h"

AClosedDoor::AClosedDoor() 
{
}

AClosedDoor::~AClosedDoor() 
{
}

void AClosedDoor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	RegisterEvent(
		EEventTriggerAction::ZClick,
		SKIP_CHECK,
		ES::Start(true)
		>> ES::Chat({ L"The door is firmly closed." }, EFontColor::Gray, 16)
		>> ES::End(true)
	);
}

