#pragma once
#include <map>
#include <vector>
#include <list>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include "PokemonString.h"
#include "Global.h"
#include "WildPokemonZone.h"

class UGameDB
{
	friend class GameDBInitiator;
public:
	// delete Function
	UGameDB(const UGameDB& _Other) = delete;
	UGameDB(UGameDB&& _Other) noexcept = delete;
	UGameDB& operator=(const UGameDB& _Other) = delete;
	UGameDB& operator=(UGameDB&& _Other) noexcept = delete;

	static const FPokemonSpecies* FindSpecies(EPokemonId _Id);
	static const FPokemonMove* FindMove(EPokemonMove _Id);
	static const FPokemonNature* FindNature(EPokemonNature _Id);
	static const FPokemonAbility* FindAbility(EPokemonAbility _Id);
	static const FPokemonGender* FindGender(EPokemonGender _Id);
	static const FPokemonType* FindType(EPokemonType _Id);
	static const FPokemonStatus* FindStatus(EPokemonStatus _Id);
	static const UWildPokemonZone* FindWildPokemonZone(std::string_view _MapName, int _Index);

	static std::list<EPokemonId> GetImplementedSpeciesNo();
	static std::string GetSpeciesName(EPokemonId _Id);

protected:

private:
	// NameResolver
	static std::map<std::string, int> NameResolver;
	static int Resolve(std::string_view _Name);

	// Æ÷ÄÏ¸ó
	static std::map<EPokemonId, FPokemonSpecies> Species;
	static std::map<EPokemonMove, FPokemonMove> Moves;
	static std::map<EPokemonNature, FPokemonNature> Natures;
	static std::map<EPokemonAbility, FPokemonAbility> Abilities;
	static std::map<EPokemonGender, FPokemonGender> Genders;
	static std::map<EPokemonStatus, FPokemonStatus> Statuses;
	static std::map<EPokemonType, FPokemonType> Types;
	static std::list<EPokemonId> ImplementedSpeciesNo;

	// Zones[¸Ê ÀÌ¸§][¹øÈ£] = (Zone °´Ã¼)
	static std::map<std::string, std::map<int, UWildPokemonZone>> WildPokemonZones;

	// constructor destructor
	UGameDB() {}
	~UGameDB() {}
};

