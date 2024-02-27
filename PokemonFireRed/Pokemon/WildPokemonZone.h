#pragma once
#include "PokemonStruct.h"

class UPokemon;

struct FWildPokemonEncounter
{
public:
	EPokedexNo Id = EPokedexNo::None;
	float Prop = 0.0f;
	int MinLevel = 1;
	int MaxLevel = 100;
};

class UWildPokemonZone
{
public:
	// constructor destructor
	UWildPokemonZone();
	~UWildPokemonZone();

	// delete Function
	UWildPokemonZone(const UWildPokemonZone& _Other) = delete;
	UWildPokemonZone(UWildPokemonZone&& _Other) noexcept = delete;
	UWildPokemonZone& operator=(const UWildPokemonZone& _Other) = delete;
	UWildPokemonZone& operator=(UWildPokemonZone&& _Other) noexcept = delete;

	void InsertWildPokemonInfo(const FWildPokemonEncounter& _Encounter)
	{
		WildPokemonEncounters.push_back(_Encounter);
	}

	UPokemon NewWildPokemon() const;

protected:
private:
	std::vector<FWildPokemonEncounter> WildPokemonEncounters;
};

