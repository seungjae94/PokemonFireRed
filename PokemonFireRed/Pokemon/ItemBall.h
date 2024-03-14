#pragma once
#include "EventTrigger.h"
#include "Item.h"

class AItemBall : public AEventTrigger
{
public:
	// constructor destructor
	AItemBall();
	~AItemBall();

	// delete Function
	AItemBall(const AItemBall& _Other) = delete;
	AItemBall(AItemBall&& _Other) noexcept = delete;
	AItemBall& operator=(const AItemBall& _Other) = delete;
	AItemBall& operator=(AItemBall&& _Other) noexcept = delete;

	void SetItem(EItemId _ItemId, int _Count = 1);
	void RegisterPredefinedEvent();

protected:

private:
	EItemId ItemId = EItemId::None;
	int Count = 0;
};

