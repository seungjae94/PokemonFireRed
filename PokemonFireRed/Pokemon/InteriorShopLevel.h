#pragma once
#include "MapLevel.h"

class AInteriorDoor;

class UInteriorShopLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorShopLevel();
	~UInteriorShopLevel();

	// delete Function
	UInteriorShopLevel(const UInteriorShopLevel& _Other) = delete;
	UInteriorShopLevel(UInteriorShopLevel&& _Other) noexcept = delete;
	UInteriorShopLevel& operator=(const UInteriorShopLevel& _Other) = delete;
	UInteriorShopLevel& operator=(UInteriorShopLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	AInteriorDoor* Door = nullptr;
	void MakeDoor();

	void SetDoorTargetAsViridianCity();
};

