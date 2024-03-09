#pragma once
#include "MapLevel.h"

class UEventTargetSetting;
class UEventCondition;
class AStarterBall;

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
	const FTileVector Up = FTileVector::Up;
	const FTileVector Down = FTileVector::Down;
	const FTileVector Left = FTileVector::Left;
	const FTileVector Right = FTileVector::Right;

	AStarterBall* BulbasaurBall = nullptr;
	AStarterBall* SquirtleBall = nullptr;
	AStarterBall* CharmanderBall = nullptr;

	void MakeDoor();
	void MakeOak();
	void MakeRivalGreen();
	void MakeSpecialTriggers();
	void SpawnSpecialTrigger(UEventTargetSetting _Setting, UEventCondition _BlockCond, UEventCondition _RivalBattleCond);
	void MakeDecorations();
};

