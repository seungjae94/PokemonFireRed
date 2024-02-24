#pragma once
#include "Page.h"
#include "PokemonText.h"
#include "PokemonIcon.h"

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

	// ������
	UPokemon* Pokemon = nullptr;
	UImageRenderer* Background = nullptr;
	UImageRenderer* CommonBox = nullptr;
	UImageRenderer* InfoBox = nullptr;
	UImageRenderer* NavInfo = nullptr;

	// CommonBox ���
	APokemonText* NameText = nullptr;
	APokemonText* LevelText = nullptr;

	// InfoBox ���
	APokemonText* PokedexNoText = nullptr;
	APokemonText* SpeciesNameText = nullptr;
	std::list<UImageRenderer*> TypeIcons;
	APokemonText* TrainerText = nullptr;
	APokemonText* IdNoText = nullptr;
	APokemonText* ItemText = nullptr;
	APokemonText* TrainerMemo = nullptr;
};

