#include "WildBattleTrigger.h"
#include "PlayerData.h"

AWildBattleTrigger::AWildBattleTrigger() 
{
}

AWildBattleTrigger::~AWildBattleTrigger() 
{
}

void AWildBattleTrigger::Update(const UPokemon& _Pokemon)
{
	UnregisterEvent();
	UPlayerData::EnemyWildPokemon = _Pokemon;
	RegisterPredefinedEvent();
}

void AWildBattleTrigger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		// 3�� �����Ÿ���
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		>> ES::FadeIn(FadeInTime)
		>> ES::Wait(FadeInTime)
		>> ES::FadeOut(FadeOutTime)
		>> ES::Wait(FadeOutTime)
		// ���� ����
		>> ES::ChangeLevel(Global::WildBattleLevel)
		>> ES::Wait(0.1f)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(false)
	);
}

void AWildBattleTrigger::UnregisterEvent()
{
	UEventManager::UnregisterEvent(this, Cond);
}
