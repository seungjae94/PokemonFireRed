#include "Warp.h"
#include <EnginePlatform/EngineInput.h>
#include "PokemonDebug.h"

AWarp::AWarp() 
{
}

AWarp::~AWarp() 
{
}

void AWarp::TriggerEvent()
{

}

void AWarp::BeginPlay()
{
	AActor::BeginPlay();
}

void AWarp::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (EngineInput::IsDown(VK_F4))
	{
		PokemonDebug::ReportPosition(this, "Warp");
	}
}

