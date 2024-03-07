#pragma once
#include "MapLevel.h"

class UInteriorOaksLabLevel : public UMapLevel
{
public:
	// constructor destructor
	UInteriorOaksLabLevel();
	~UInteriorOaksLabLevel();

	// delete Function
	UInteriorOaksLabLevel(const UInteriorOaksLabLevel& _Other) = delete;
	UInteriorOaksLabLevel(UInteriorOaksLabLevel&& _Other) noexcept = delete;
	UInteriorOaksLabLevel& operator=(const UInteriorOaksLabLevel& _Other) = delete;
	UInteriorOaksLabLevel& operator=(UInteriorOaksLabLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:
	bool FightWithGreen = false;
};

