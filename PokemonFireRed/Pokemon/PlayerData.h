#pragma once
#include <vector>
#include "Pokemon.h"

class UPlayerData
{
public:
	// constructor destructor
	UPlayerData();
	~UPlayerData();

	static UPokemon& GetPokemonInEntry(int _Index);
	static void AddPokemonToEntry(const UPokemon& _Pokemon);
	static void AddPokemonToComputer(const UPokemon& _Pokemon);

protected:

private:
	static std::vector<UPokemon> PokemonEntry;
	static std::vector<UPokemon> PokemonInComputer;

};

