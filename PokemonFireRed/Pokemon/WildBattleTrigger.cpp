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
		// 3¹ø ±ôºý°Å¸®±â
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
		// ·¹º§ º¯°æ
		>> ES::ChangeLevel(Global::BattleLevel)
		>> ES::FadeIn(0.5f, EFadeType::VCurtain)
		>> ES::Wait(0.5f)
		>> ES::End(false)
	);
}

void AWildBattleTrigger::UnregisterEvent()
{
	UEventManager::UnregisterEvent(this, Cond);
}
