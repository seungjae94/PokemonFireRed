#pragma once
#include "MapLevel.h"

class UExteriorPewterCityLevel : public UMapLevel
{
public:
	// constructor destructor
	UExteriorPewterCityLevel();
	~UExteriorPewterCityLevel();

	// delete Function
	UExteriorPewterCityLevel(const UExteriorPewterCityLevel& _Other) = delete;
	UExteriorPewterCityLevel(UExteriorPewterCityLevel&& _Other) noexcept = delete;
	UExteriorPewterCityLevel& operator=(const UExteriorPewterCityLevel& _Other) = delete;
	UExteriorPewterCityLevel& operator=(UExteriorPewterCityLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;

	void MakeForestEntrances();
	void MakePokemonCenterDoor();
	void MakeGymDoor();
	void MakeAnimatedTiles();
	void MakeShopClosedDoor();
	void MakePrivateHouse1ClosedDoor();
	void MakePrivateHouse2ClosedDoor();
	void MakeMuseumClosedDoor();
};

