#include "EventActor.h"

AEventActor::AEventActor() 
{
}

AEventActor::~AEventActor() 
{
}

void AEventActor::SetTilePoint(const FTileVector& _Point)
{
	FVector Pos = _Point.ToFVector();
	SetActorLocation(Pos);

	FTileVector CurPoint = FTileVector(GetActorLocation());

	std::map<FTileVector, AEventActor*>& AllEventActor = MapLevel->AllEventActor;
	if (false == AllEventActor.contains(CurPoint))
	{
		std::map<FTileVector, AEventActor*>::iterator FindIter = AllEventActor.find(CurPoint);
		AllEventActor.erase(FindIter);
	}

	AllEventActor[_Point] = this;
}

