#pragma once
#include "Page.h"
#include "PokemonText.h"
#include "PokemonIcon.h"

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

	void SetPokemon(UPokemon* _Pokemon)
	{
		Pokemon = _Pokemon;
	}

	void RefreshAll();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	UPokemon* Pokemon = nullptr;
	UImageRenderer* Background = nullptr;
	UImageRenderer* CommonBox = nullptr;
	UImageRenderer* InfoBox = nullptr;
	UImageRenderer* NavInfo = nullptr;

	// CommonBox 요소
	APokemonText* NameText = nullptr;
	APokemonText* LevelText = nullptr;

	// InfoBox 요소
	APokemonText* PokedexNoText = nullptr;
	APokemonText* SpeciesNameText = nullptr;
	std::list<UImageRenderer*> TypeIcons;
	APokemonText* TrainerText = nullptr;
	APokemonText* IdNoText = nullptr;
	APokemonText* ItemText = nullptr;
	APokemonText* TrainerMemo = nullptr;
};

