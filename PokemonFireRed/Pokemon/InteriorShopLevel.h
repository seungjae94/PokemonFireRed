#pragma once
#include "MapLevel.h"

class AInteriorDoor;

class InteriorShopLevel : public UMapLevel
{
public:
	// constructor destructor
	InteriorShopLevel();
	~InteriorShopLevel();

	// delete Function
	InteriorShopLevel(const InteriorShopLevel& _Other) = delete;
	InteriorShopLevel(InteriorShopLevel&& _Other) noexcept = delete;
	InteriorShopLevel& operator=(const InteriorShopLevel& _Other) = delete;
	InteriorShopLevel& operator=(InteriorShopLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	AInteriorDoor* Door = nullptr;
	void MakeDoor();

	void SetDoorTargetAsViridianCity();
};

