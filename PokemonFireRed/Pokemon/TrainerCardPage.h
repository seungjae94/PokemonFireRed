#pragma once
#include "Page.h"

class ATrainerCardPage : public APage
{
public:
	// constructor destructor
	ATrainerCardPage();
	~ATrainerCardPage();

	// delete Function
	ATrainerCardPage(const ATrainerCardPage& _Other) = delete;
	ATrainerCardPage(ATrainerCardPage&& _Other) noexcept = delete;
	ATrainerCardPage& operator=(const ATrainerCardPage& _Other) = delete;
	ATrainerCardPage& operator=(ATrainerCardPage&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	std::string PrevMapLevelName;

	UImageRenderer* Background = nullptr;
};

