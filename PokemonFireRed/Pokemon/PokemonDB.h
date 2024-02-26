#pragma once
#include <map>
#include <list>
#include <EngineBase/EngineString.h>
#include "Global.h"
#include "PokemonStruct.h"

class UPokemonDB
{
	friend class PokemonDBInitiator;
public:
	// delete Function
	UPokemonDB(const UPokemonDB& _Other) = delete;
	UPokemonDB(UPokemonDB&& _Other) noexcept = delete;
	UPokemonDB& operator=(const UPokemonDB& _Other) = delete;
	UPokemonDB& operator=(UPokemonDB&& _Other) noexcept = delete;

	static const FPokemonSpecies* FindSpecies(EPokedexNo _Id)
	{
		return &Species[_Id];
	}

	static const FPokemonMove* FindMove(EPokemonMove _Id)
	{
		return &Moves[_Id];
	}

	static const FPokemonNature* FindNature(EPokemonNature _Id)
	{
		return &Natures[_Id];
	}

	static const FPokemonAbility* FindAbility(EPokemonAbility _Id)
	{
		return &Abilities[_Id];
	}

	static const FPokemonGender* FindGender(EPokemonGender _Id)
	{
		return &Genders[_Id];
	}

	static const FPokemonType* FindType(EPokemonType _Id)
	{
		return &Types[_Id];
	}

	static std::list<EPokedexNo> GetImplementedSpeciesNo()
	{
		return ImplementedSpeciesNo;
	}

	static std::string GetSpeciesName(EPokedexNo _Id)
	{
		return Species[_Id].SpeciesName;
	}

protected:

private:
	static std::map<EPokedexNo, FPokemonSpecies> Species;
	static std::map<EPokemonMove, FPokemonMove> Moves;
	static std::map<EPokemonNature, FPokemonNature> Natures;
	static std::map<EPokemonAbility, FPokemonAbility> Abilities;
	static std::map<EPokemonGender, FPokemonGender> Genders;
	static std::map<EPokemonStatus, FPokemonStatus> Status;
	static std::map<EPokemonType, FPokemonType> Types;

	static std::list<EPokedexNo> ImplementedSpeciesNo;

	// constructor destructor
	UPokemonDB() {}
	~UPokemonDB() {}
};

