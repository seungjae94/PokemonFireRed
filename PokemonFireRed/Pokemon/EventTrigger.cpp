#include "EventTrigger.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include "EventManager.h"

AEventTrigger::AEventTrigger() 
{
}

AEventTrigger::~AEventTrigger() 
{
}

//void AEventTrigger::SetTilePoint(const FTileVector& _Point)
//{
//	FVector Pos = _Point.ToFVector();
//	SetActorLocation(Pos);
//
//	FTileVector CurPoint = FTileVector(GetActorLocation());
//
//	std::map<FTileVector, AEventTrigger*>& AllEventActor = MapLevel->AllEventActor;
//	if (false == AllEventActor.contains(CurPoint))
//	{
//		std::map<FTileVector, AEventTrigger*>::iterator FindIter = AllEventActor.find(CurPoint);
//		AllEventActor.erase(FindIter);
//	}
//
//	AllEventActor[_Point] = this;
//}