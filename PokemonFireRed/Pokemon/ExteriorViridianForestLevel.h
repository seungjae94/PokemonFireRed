#pragma once
#include "MapLevel.h"

class ATrainer;

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
	ATrainer* BugCatcher0 = nullptr;
	ATrainer* BugCatcher1 = nullptr;

	void MakeBugCatcher0();
	void MakeBugCatcher1();
	void MakeExitToR2();
	void MakeExitsToPewter();

	static bool FightWithBugCatcher0Checker();
	static bool FightWithBugCatcher1Checker();

	static std::vector<FTileVector> BugCatcher0PathGenerator();
	static std::vector<FTileVector> BugCatcher1PathGenerator();
};

