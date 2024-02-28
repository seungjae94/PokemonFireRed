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
		ES::Start(false)
		// 4�� �����Ÿ���
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
		// ���� ����
		>> ES::ChangeLevel(Global::BattleLevel)
		>> ES::End(false)
	);
}

void AWildBattleTrigger::UnregisterEvent()
{
	UEventManager::UnregisterEvent(this, Cond);
}
