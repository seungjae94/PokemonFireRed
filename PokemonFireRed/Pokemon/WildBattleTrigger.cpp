#include "WildBattleTrigger.h"
#include "UserData.h"

AWildBattleTrigger::AWildBattleTrigger()
{
}

AWildBattleTrigger::~AWildBattleTrigger()
{
}

void AWildBattleTrigger::Update(const FWildPokemonConstructorParam& _ConstructorParam)
{
	UnregisterEvent();

	Entry.clear();
	Entry.push_back(UPokemon(_ConstructorParam.Id, _ConstructorParam.Level));
	UEventManager::SaveEnemyEntry(&Entry);
	UEventManager::SetAsWildPokemonBattle();

	RegisterPredefinedEvent();
}

void AWildBattleTrigger::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();
	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::PlayBgm(RN::BgmWildBattle)
		// 2¹ø ±ôºý°Å¸®±â
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
