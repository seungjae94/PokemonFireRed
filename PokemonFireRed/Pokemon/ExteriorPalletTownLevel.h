#pragma once
#include "MapLevel.h"

// ���� :
class UExteriorPalletTownLevel : public UMapLevel
{
public:
	// constructor destructor
	UExteriorPalletTownLevel();
	~UExteriorPalletTownLevel();

	// delete Function
	UExteriorPalletTownLevel(const UExteriorPalletTownLevel& _Other) = delete;
	UExteriorPalletTownLevel(UExteriorPalletTownLevel&& _Other) noexcept = delete;
	UExteriorPalletTownLevel& operator=(const UExteriorPalletTownLevel& _Other) = delete;
	UExteriorPalletTownLevel& operator=(UExteriorPalletTownLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
private:
	// ���� ����
	void MakePalletTown();
	void MakePTOaksLabDoor();
	void MakePTPlayersHouseDoor();
	void MakePTRivalsHouseDoor();
	void MakePTTechMan();
	void MakePTAnimatedTiles();
};

