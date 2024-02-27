#include "WildPokemonZone.h"
#include "PokemonMath.h"
#include "Pokemon.h"

UWildPokemonZone::UWildPokemonZone()
{
}

UWildPokemonZone::~UWildPokemonZone()
{
}

UPokemon UWildPokemonZone::NewWildPokemon() const
{
	float RandomNumber = UPokemonMath::Random(0.0f, 1.0f);

	EPokedexNo Id = WildPokemonEncounters[0].Id;
	int Level = WildPokemonEncounters[0].MinLevel;
	for (int i = 0; i < WildPokemonEncounters.size(); ++i)
	{
		const FWildPokemonEncounter& Info = WildPokemonEncounters[i];

		RandomNumber -= Info.Prop;

		if (RandomNumber < 0)
		{
			Id = Info.Id;
			Level = UPokemonMath::RandomInt(Info.MinLevel, Info.MaxLevel);
			break;
		}
	}

	return UPokemon(Id, Level);
}
