#include "PlayerData.h"

std::vector<UPokemon> UPlayerData::PokemonEntry;
std::vector<UPokemon> UPlayerData::PokemonInComputer;
std::map<EAchievement, bool> UPlayerData::AchievementMap;

UPlayerData::UPlayerData() 
{
}

UPlayerData::~UPlayerData() 
{
}

UPokemon& UPlayerData::GetPokemonInEntry(int _Index)
{
	return PokemonEntry[_Index];
}

void UPlayerData::AddPokemonToEntry(const UPokemon& _Pokemon)
{
	if (PokemonEntry.size() == 6)
	{
		AddPokemonToComputer(_Pokemon);
		return;
	}

	PokemonEntry.push_back(_Pokemon);
}

void UPlayerData::AddPokemonToComputer(const UPokemon& _Pokemon)
{
	PokemonInComputer.push_back(_Pokemon);
}



