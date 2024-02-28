#include "WildBattleTrigger.h"

AWildBattleTrigger::AWildBattleTrigger() 
{
}

AWildBattleTrigger::~AWildBattleTrigger() 
{
}

void AWildBattleTrigger::Update(const UPokemon& _Pokemon)
{
	UnregisterEvent();
	Pokemon = _Pokemon;
	RegisterPredefinedEvent();
}

void AWildBattleTrigger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		// 4¹ø ±ôºý°Å¸®±â
		>> ES::FadeOut(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeIn(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeOut(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeIn(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeOut(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeIn(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeOut(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeIn(BlinkTime)
		>> ES::Wait(BlinkTime)
		>> ES::FadeOut(0.5f)
		// ·¹º§ º¯°æ
		>> ES::ChangeLevel(Global::BattleLevel)
		>> ES::Wait(0.3f)
		>> ES::FadeIn(0.5f)
		>> ES::End(false)
	);
}

void AWildBattleTrigger::UnregisterEvent()
{
	UEventManager::UnregisterEvent(this, Cond);
}
