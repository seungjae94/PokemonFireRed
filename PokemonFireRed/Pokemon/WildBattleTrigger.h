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

	void Trigger(const FWildPokemonConstructorParam & _ConstructorParam)
	{
		Update(_ConstructorParam);
		Entry.clear();
		Entry.push_back(UPokemon(_ConstructorParam.Id, _ConstructorParam.Level));
		UEventManager::SaveEnemyEntry(&Entry);
		UEventManager::TriggerEvent(this, EEventTriggerAction::Direct);
	}

protected:

private:
	UEventCondition Cond;
	const float FadeOutTime = 0.25f;
	const float FadeInTime = 0.15f;

	std::vector<UPokemon> Entry;

	void Update(const FWildPokemonConstructorParam & _ConstructorParam);
	void RegisterPredefinedEvent() override;
	void UnregisterEvent();
};

