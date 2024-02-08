#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"
#include "MapLevel.h"

class APlayer;

// 항상 플레이어에 관한 조건 변화를 관찰하면서 자신의 상태를 변경하고 플레이어와 상호 작용하는 액터
class AEventActor : public AActor
{
public:
	// constructor destructor
	AEventActor();
	~AEventActor();

	// delete Function
	AEventActor(const AEventActor& _Other) = delete;
	AEventActor(AEventActor&& _Other) noexcept = delete;
	AEventActor& operator=(const AEventActor& _Other) = delete;
	AEventActor& operator=(AEventActor&& _Other) noexcept = delete;

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

	APlayer* GetPlayer()
	{
		return Player;
	}

	virtual void TriggerEvent() {};

	void SetTilePoint(const FTileVector& _Point);

	void SetMapLevel(UMapLevel* _MapLevel)
	{
		MapLevel = _MapLevel;
	}

protected:
	APlayer* Player;
private:
	UMapLevel* MapLevel;
};

