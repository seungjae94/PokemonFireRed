#include "Battler.h"
#include "PlayerData.h"

UBattler::UBattler()
{
}

UBattler::~UBattler()
{
}

void UBattler::SetPlayer()
{
	IsPlayerValue = true;

	int PlayerEntrySize = static_cast<int>(UPlayerData::GetPokemonEntrySize());
	for (int i = 0; i < PlayerEntrySize; ++i)
	{
		Entry.push_back(&UPlayerData::GetPokemonInEntry(i));
	}
}

void UBattler::SetWildPokemon()
{
	IsWildPokemonValue = true;

	Entry.push_back(UPlayerData::GetEnemyWildPokemon());
}
