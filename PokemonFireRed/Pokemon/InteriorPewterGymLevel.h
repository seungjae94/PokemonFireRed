#pragma once
#include "MapLevel.h"

class UInteriorPewterGymLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorPewterGymLevel();
	~UInteriorPewterGymLevel();

	// delete Function
	UInteriorPewterGymLevel(const UInteriorPewterGymLevel& _Other) = delete;
	UInteriorPewterGymLevel(UInteriorPewterGymLevel&& _Other) noexcept = delete;
	UInteriorPewterGymLevel& operator=(const UInteriorPewterGymLevel& _Other) = delete;
	UInteriorPewterGymLevel& operator=(UInteriorPewterGymLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;

	void MakePewterCityDoor();
};

