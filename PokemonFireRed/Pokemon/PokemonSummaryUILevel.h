#pragma once
#include "PokemonLevel.h"
#include "PokemonSummaryPage.h"

class UPokemonSummaryUILevel : public UPokemonLevel
{
public:
	// constructor destructor
	UPokemonSummaryUILevel();
	~UPokemonSummaryUILevel();

	// delete Function
	UPokemonSummaryUILevel(const UPokemonSummaryUILevel& _Other) = delete;
	UPokemonSummaryUILevel(UPokemonSummaryUILevel&& _Other) noexcept = delete;
	UPokemonSummaryUILevel& operator=(const UPokemonSummaryUILevel& _Other) = delete;
	UPokemonSummaryUILevel& operator=(UPokemonSummaryUILevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	APokemonSummaryPage* Page = nullptr;
};

