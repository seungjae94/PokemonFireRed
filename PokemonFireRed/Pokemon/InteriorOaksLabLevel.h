#pragma once
#include "MapLevel.h"

class UEventTargetSetting;
class UEventCondition;
class AStarterBall;
class ATrainer;

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
private:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	const FTileVector Up = FTileVector::Up;
	const FTileVector Down = FTileVector::Down;
	const FTileVector Left = FTileVector::Left;
	const FTileVector Right = FTileVector::Right;
	const float FadeOutTime = 0.25f;
	const float FadeInTime = 0.15f;

	AStarterBall* BulbasaurBall = nullptr;
	AStarterBall* SquirtleBall = nullptr;
	AStarterBall* CharmanderBall = nullptr;
	ATrainer* Green = nullptr;
	std::vector<FTileVector> GreenBattleDynamicPath;

	void MakeDoor();
	void MakeOak();
	void MakeRivalGreen();
	void MakeSpecialTriggers();
	void SpawnSpecialTrigger(UEventTargetSetting _Setting, UEventCondition _BlockCond, UEventCondition _RivalBattleCond);
	void MakeDecorations();
};

