#pragma once
#include "PokemonLevel.h"
#include "FadeLevelChanger.h"
#include "PokemonUILevelPage.h"

class UPokemonUILevel : public UPokemonLevel
{
public:
	// constructor destructor
	UPokemonUILevel();
	~UPokemonUILevel();

	// delete Function
	UPokemonUILevel(const UPokemonUILevel& _Other) = delete;
	UPokemonUILevel(UPokemonUILevel&& _Other) noexcept = delete;
	UPokemonUILevel& operator=(const UPokemonUILevel& _Other) = delete;
	UPokemonUILevel& operator=(UPokemonUILevel&& _Other) noexcept = delete;

	UPokemon* GetTargetPokemon()
	{
		return Manager->GetTargetPokemon();
	}

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
private:
	UPokemonUILevelPage* Manager = nullptr;
	AFadeLevelChanger* LevelChanger = nullptr;
};

