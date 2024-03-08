#include "EventStream.h"
#include "Trainer.h"
#include "WildBattleTrigger.h"
#include "Pokemon.h"

UEventStream::UEventStream() 
{
}

UEventStream::~UEventStream() 
{
}

UEventStream::TrainerBattle::TrainerBattle(ATrainer* _Trainer)
{
	Entry = _Trainer->GetEntry();
	TrainerName = _Trainer->GetBattlerName();
	TrainerImageName = _Trainer->GetBattlerImageName();
}

UEventStream::WildBattle::WildBattle(AWildBattleTrigger* _Wild)
{
	//Entry = _Wild->GetEntry();
}
