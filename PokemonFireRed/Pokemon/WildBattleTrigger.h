#pragma once
#include "EventTrigger.h"
#include "Pokemon.h"

class AWildBattleTrigger : public AEventTrigger
{
public:
	// constructor destructor
	AWildBattleTrigger();
	~AWildBattleTrigger();

	// delete Function
	AWildBattleTrigger(const AWildBattleTrigger& _Other) = delete;
	AWildBattleTrigger(AWildBattleTrigger&& _Other) noexcept = delete;
	AWildBattleTrigger& operator=(const AWildBattleTrigger& _Other) = delete;
	AWildBattleTrigger& operator=(AWildBattleTrigger&& _Other) noexcept = delete;

	void Trigger(const UPokemon& _Pokemon)
	{
		Update(Pokemon);
		UEventManager::TriggerEvent(this, EEventTriggerAction::Direct);
	}

protected:

private:
	UEventCondition Cond;
	UPokemon Pokemon;
	float BlinkTime = 0.15f;

	void Update(const UPokemon& _Pokemon);
	void RegisterPredefinedEvent() override;
	void UnregisterEvent();
};

