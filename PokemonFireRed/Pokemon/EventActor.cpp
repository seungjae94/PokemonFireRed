#include "EventActor.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"
#include "EventManager.h"

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

void AEventActor::BeginPlay()
{
	AActor::BeginPlay();
	EventProcessor = UEventManager::CreateEventProcessor(this);
}

void AEventActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	// 디버그 기능
	if (UEngineInput::IsDown(VK_F4))
	{
		PokemonDebug::ReportPosition(this, "EventActor");
	}
}