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

void UPlayerData::SwapEntry(int _Index0, int _Index1)
{
	if (_Index0 < 0 || _Index0 >= PokemonEntry.size())
	{
		MsgBoxAssert("Index0: " + std::to_string(_Index0) + "´Â ¿£Æ®¸® ÀÎµ¦½º¸¦ ¹þ¾î³µ½À´Ï´Ù.");
		return;
	}

	if (_Index1 < 0 || _Index1 >= PokemonEntry.size())
	{
		MsgBoxAssert("Index1: " + std::to_string(_Index1) + "´Â ¿£Æ®¸® ÀÎµ¦½º¸¦ ¹þ¾î³µ½À´Ï´Ù.");
		return;
	}

	UPokemon& Tmp = PokemonEntry[0];
	PokemonEntry[1] = PokemonEntry[0];
	PokemonEntry[1] = Tmp;
}



