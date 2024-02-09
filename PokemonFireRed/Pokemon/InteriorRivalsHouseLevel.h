#pragma once
#include "MapLevel.h"

class UInteriorRivalsHouseLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorRivalsHouseLevel();
	~UInteriorRivalsHouseLevel();

	// delete Function
	UInteriorRivalsHouseLevel(const UInteriorRivalsHouseLevel& _Other) = delete;
	UInteriorRivalsHouseLevel(UInteriorRivalsHouseLevel&& _Other) noexcept = delete;
	UInteriorRivalsHouseLevel& operator=(const UInteriorRivalsHouseLevel& _Other) = delete;
	UInteriorRivalsHouseLevel& operator=(UInteriorRivalsHouseLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:

};

