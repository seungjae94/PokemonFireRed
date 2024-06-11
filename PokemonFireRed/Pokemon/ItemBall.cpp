#include "ItemBall.h"

AItemBall::AItemBall() 
{
}

AItemBall::~AItemBall() 
{
}

void AItemBall::SetItem(EItemId _ItemId, int _Count)
{
	ItemId = _ItemId;
	Count = _Count;
}

void AItemBall::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond = UEventCondition(EEventTriggerAction::ZClick);

	const FItem* Item = UGameData::FindItem(ItemId);

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::PlaySE(RN::SEItemObtained, Global::LevelUpFanfareTime)
		>> ES::Chat({ L"RED found " + std::to_wstring(Count) + L" " + Item->Name + L"!"}, EFontColor::Gray, 16)
		>> ES::GainItem(ItemId, Count)
		>> ES::Destroy(this)
		>> ES::End(true)
	);
}

