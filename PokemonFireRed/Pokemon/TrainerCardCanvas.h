#pragma once
#include "Canvas.h"

class ATrainerCardCanvas : public ACanvas
{
public:
	// constructor destructor
	ATrainerCardCanvas();
	~ATrainerCardCanvas();

	// delete Function
	ATrainerCardCanvas(const ATrainerCardCanvas& _Other) = delete;
	ATrainerCardCanvas(ATrainerCardCanvas&& _Other) noexcept = delete;
	ATrainerCardCanvas& operator=(const ATrainerCardCanvas& _Other) = delete;
	ATrainerCardCanvas& operator=(ATrainerCardCanvas&& _Other) noexcept = delete;

	void SetPrevMapLevelName(std::string_view _PrevMapLevelName)
	{
		PrevMapLevelName = _PrevMapLevelName;
	}

	void RefreshAll();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	std::string PrevMapLevelName;

	UImageRenderer* Background = nullptr;
	AText* IdNo = nullptr;
	AText* Nickname = nullptr;
	AText* Money = nullptr;
	AText* PokedexOwned = nullptr;
};

