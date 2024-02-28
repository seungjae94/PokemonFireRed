#pragma once
#include "PokemonLevel.h"
#include "TrainerCardCanvas.h"

class UTrainerCardUILevel : public UPokemonLevel
{
public:
	// constructor destructor
	UTrainerCardUILevel();
	~UTrainerCardUILevel();

	// delete Function
	UTrainerCardUILevel(const UTrainerCardUILevel& _Other) = delete;
	UTrainerCardUILevel(UTrainerCardUILevel&& _Other) noexcept = delete;
	UTrainerCardUILevel& operator=(const UTrainerCardUILevel& _Other) = delete;
	UTrainerCardUILevel& operator=(UTrainerCardUILevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
private:
	ATrainerCardCanvas* Canvas = nullptr;
};

