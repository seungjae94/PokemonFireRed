#include "DialogueActor.h"
#include "EventManager.h"
#include "EventCondition.h"
#include "EventStream.h"
#include "PokemonText.h"
#include "Player.h"
#include "PokemonLevel.h"

ADialogueActor::ADialogueActor()
{
}

ADialogueActor::~ADialogueActor()
{
}

void ADialogueActor::BeginPlay()
{
	AEventTrigger::BeginPlay();
}

void ADialogueActor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond1 = UEventCondition(EEventTriggerAction::ZClick);

	if (false == Rotatable)
	{
		UEventManager::RegisterEvent(this, Cond1,
			ES::Start(true)
			>> ES::Chat(Dialogue, TextColor, 16, true)
			>> ES::End(true)
		);
		return;
	}

	UEventManager::RegisterEvent(this, Cond1,
		ES::Start(true)
		>> ES::StarePlayer(GetName())
		>> ES::Chat(Dialogue, TextColor, 16, true)
		>> ES::End(true)
	);
}

ADialogueActor* ADialogueActor::GenerateObject(UPokemonLevel* _Level, std::string_view _Name, const FTileVector _Point, EFontColor _TextColor, const std::vector<std::wstring>& _Dialogue)
{
	UEventTargetInit DAInit;
	DAInit.SetName(_Name);
	DAInit.SetPoint(_Point);

	ADialogueActor* DA = _Level->SpawnEventTrigger<ADialogueActor>(DAInit);
	DA->SetTextColor(_TextColor);
	DA->SetDialogue(_Dialogue);
	DA->RegisterPredefinedEvent();

	return DA;

}
