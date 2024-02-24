#pragma once
#include "Page.h"

class APokemonSummaryPage : public APage
{
public:
	// constructor destructor
	APokemonSummaryPage();
	~APokemonSummaryPage();

	// delete Function
	APokemonSummaryPage(const APokemonSummaryPage& _Other) = delete;
	APokemonSummaryPage(APokemonSummaryPage&& _Other) noexcept = delete;
	APokemonSummaryPage& operator=(const APokemonSummaryPage& _Other) = delete;
	APokemonSummaryPage& operator=(APokemonSummaryPage&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UImageRenderer* Background = nullptr;
	UImageRenderer* CommonBox = nullptr;
	UImageRenderer* InfoBox = nullptr;
	UImageRenderer* NavInfo = nullptr;

};

