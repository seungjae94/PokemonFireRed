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
}

UEventStream::WildBattle::WildBattle(AWildBattleTrigger* _Wild)
{
	//Entry = _Wild->GetEntry();
}
