#pragma once
#include "MapLevel.h"

class AInteriorDoor;

class UInteriorPokemonCenterLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorPokemonCenterLevel();
	~UInteriorPokemonCenterLevel();

	// delete Function
	UInteriorPokemonCenterLevel(const UInteriorPokemonCenterLevel& _Other) = delete;
	UInteriorPokemonCenterLevel(UInteriorPokemonCenterLevel&& _Other) noexcept = delete;
	UInteriorPokemonCenterLevel& operator=(const UInteriorPokemonCenterLevel& _Other) = delete;
	UInteriorPokemonCenterLevel& operator=(UInteriorPokemonCenterLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _PrevLevel) override;

	AInteriorDoor* Door = nullptr;
	void MakeDoor();
	void MakeNurse();

	void SetDoorTargetAsViridianCity();
	//void SetDoorTargetAsPewterCity();
};

