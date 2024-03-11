#pragma once
#include "MapLevel.h"

class UExteriorViridianForestLevel : public UMapLevel
{
public:
	// constructor destructor
	UExteriorViridianForestLevel();
	~UExteriorViridianForestLevel();

	// delete Function
	UExteriorViridianForestLevel(const UExteriorViridianForestLevel& _Other) = delete;
	UExteriorViridianForestLevel(UExteriorViridianForestLevel&& _Other) noexcept = delete;
	UExteriorViridianForestLevel& operator=(const UExteriorViridianForestLevel& _Other) = delete;
	UExteriorViridianForestLevel& operator=(UExteriorViridianForestLevel&& _Other) noexcept = delete;

protected:

private:
	void BeginPlay() override;

	void MakeExitToR2();
	void MakeExitsToPewter();
};

