#pragma once
#include "EventTrigger.h"

class AClosedDoor : public AEventTrigger
{
public:
	// constructor destructor
	AClosedDoor();
	~AClosedDoor();

	// delete Function
	AClosedDoor(const AClosedDoor& _Other) = delete;
	AClosedDoor(AClosedDoor&& _Other) noexcept = delete;
	AClosedDoor& operator=(const AClosedDoor& _Other) = delete;
	AClosedDoor& operator=(AClosedDoor&& _Other) noexcept = delete;

	void RegisterPredefinedEvent();

protected:

private:

};

