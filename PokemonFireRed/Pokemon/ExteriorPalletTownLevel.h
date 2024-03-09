#pragma once
#include "MapLevel.h"

class UEventTargetSetting;

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
	// 태초 마을
	void MakePalletTown();
	void MakePTOaksLabDoor();
	void MakePTPlayersHouseDoor();
	void MakePTRivalsHouseDoor();
	void MakePTTechMan();
	void MakePTGetStarterEventTriggers();
	void SpawnPTGetStarterEventTrigger(
		UEventTargetSetting _Setting,
		UEventCondition _Cond,
		const std::vector<FTileVector>& _OakComePath, 
		const std::vector<FTileVector>& _OakGoToLabPath, 
		const std::vector<FTileVector>& _PlayerGoToLabPath);
	void MakePTOak();
	void MakePTAnimatedTiles();
};

