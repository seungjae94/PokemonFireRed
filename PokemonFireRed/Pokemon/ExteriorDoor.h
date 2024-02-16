#pragma once
#include "Warp.h"

class AExteriorDoor : public AWarp
{
public:
	// constructor destructor
	AExteriorDoor();
	~AExteriorDoor();

	// delete Function
	AExteriorDoor(const AExteriorDoor& _Other) = delete;
	AExteriorDoor(AExteriorDoor&& _Other) noexcept = delete;
	AExteriorDoor& operator=(const AExteriorDoor& _Other) = delete;
	AExteriorDoor& operator=(AExteriorDoor&& _Other) noexcept = delete;

	void RegisterPredefinedEvent() override;

protected:

private:
};

