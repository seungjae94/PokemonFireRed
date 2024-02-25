#pragma once
#include "MapLevel.h"

class UInteriorPlayersHouse2FLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorPlayersHouse2FLevel();
	~UInteriorPlayersHouse2FLevel();

	// delete Function
	UInteriorPlayersHouse2FLevel(const UInteriorPlayersHouse2FLevel& _Other) = delete;
	UInteriorPlayersHouse2FLevel(UInteriorPlayersHouse2FLevel&& _Other) noexcept = delete;
	UInteriorPlayersHouse2FLevel& operator=(const UInteriorPlayersHouse2FLevel& _Other) = delete;
	UInteriorPlayersHouse2FLevel& operator=(UInteriorPlayersHouse2FLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
private:

};

