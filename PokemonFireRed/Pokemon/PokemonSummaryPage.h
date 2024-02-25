#pragma once
#include "Page.h"
#include "Text.h"
#include "PokemonElement.h"

enum class EPokemonSummaryPageState
{
	Info,
	Skills,
	Moves,
	MovesDetail,
};

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
	// FSM
	EPokemonSummaryPageState State = EPokemonSummaryPageState::Info;
	void InfoTick(float _DeltaTime);

	// 데이터
	UPokemon* Pokemon = nullptr;
	UImageRenderer* Background = nullptr;
	UImageRenderer* CommonBox = nullptr;
	UImageRenderer* InfoBox = nullptr;
	UImageRenderer* NavInfo = nullptr;

	// CommonBox 요소
	AText* NameText = nullptr;
	AText* LevelText = nullptr;
	UImageRenderer* GenderIcon = nullptr;
	AImageElement* FrontImage = nullptr;

	// InfoBox 요소
	AText* PokedexNoText = nullptr;
	AText* SpeciesNameText = nullptr;
	std::list<UImageRenderer*> TypeIcons;
	AText* TrainerText = nullptr;
	AText* IdNoText = nullptr;
	AText* ItemText = nullptr;
	AText* TrainerMemo = nullptr;
};

