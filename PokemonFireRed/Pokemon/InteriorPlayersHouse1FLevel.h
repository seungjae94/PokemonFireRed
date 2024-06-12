#pragma once
#include "MapLevel.h"

class UInteriorPlayersHouse1FLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorPlayersHouse1FLevel();
	~UInteriorPlayersHouse1FLevel();

	// delete Function
	UInteriorPlayersHouse1FLevel(const UInteriorPlayersHouse1FLevel& _Other) = delete;
	UInteriorPlayersHouse1FLevel(UInteriorPlayersHouse1FLevel&& _Other) noexcept = delete;
	UInteriorPlayersHouse1FLevel& operator=(const UInteriorPlayersHouse1FLevel& _Other) = delete;
	UInteriorPlayersHouse1FLevel& operator=(UInteriorPlayersHouse1FLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:
	void MakeStair();
	void MakeDoor();
	void MakeMom();
	void MakeDecorations();
};

