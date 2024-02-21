#pragma once
#include <vector>
#include <map>
#include "Pokemon.h"

enum class EAchievement
{
	GetFirstPokemon,
	GetPokedex,

};

class UPlayerData
{
public:
	// constructor destructor
	UPlayerData();
	~UPlayerData();

	static UPokemon& GetPokemonInEntry(int _Index);
	static void AddPokemonToEntry(const UPokemon& _Pokemon);
	static void AddPokemonToComputer(const UPokemon& _Pokemon);
	
	static bool IsAchieved(EAchievement _Achievement)
	{
		return AchievementMap[_Achievement];
	}

	static void Achieve(EAchievement _Achievement)
	{
		AchievementMap[_Achievement] = true;
	}

protected:

private:
	static std::vector<UPokemon> PokemonEntry;
	static std::vector<UPokemon> PokemonInComputer;
	static std::map<EAchievement, bool> AchievementMap;
};

