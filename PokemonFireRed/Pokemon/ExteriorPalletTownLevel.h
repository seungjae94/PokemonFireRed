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
	const FTileVector Up = FTileVector::Up;
	const FTileVector Down = FTileVector::Down;
	const FTileVector Left = FTileVector::Left;
	const FTileVector Right = FTileVector::Right;

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

	// 상록 시티
	void MakeViridianCity();
	void MakeVCPokemonCenterDoor();
	void MakeVCShopDoor();
	void MakeVCShopClosedDoor();
	void MakeVCTrainerSchoolClosedDoor();
	void MakeVCPrivateHouseClosedDoor();
	void MakeVCGymClosedDoor();
	void MakeVCAnimatedTiles();
	void MakeVCForestEntrances();

	// 22번 도로
	void MakeRoute22();
	void MakeR22AnimatedTiles();
	void MakeR22ClosedDoor();
};

