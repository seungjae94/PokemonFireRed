#include "EventActor.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"

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

void AEventActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	// 디버그 기능
	if (UEngineInput::IsDown(VK_F4))
	{
		PokemonDebug::ReportPosition(this, "EventActor");
	}

	if (false == IsTriggered)
	{
		return;
	}

	bool EventEnd = AllEvents[CurEventIndex](this, _DeltaTime);
	
	if (true == EventEnd)
	{
		// 더이상 수행할 이벤트가 없는 경우
		if (CurEventIndex + 1 >= AllEvents.size())
		{
			CurEventIndex = 0;
			IsTriggered = false;
			return;
		}

		CurEventIndex++;
	}
}