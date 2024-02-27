#pragma once
#include <map>
#include <vector>
#include <list>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include "Global.h"
#include "WildPokemonZone.h"

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

	static const UWildPokemonZone* FindWildPokemonZone(std::string_view _MapName, int _Index)
	{
		return &WildPokemonZones[_MapName.data()][_Index];
	}

	static std::list<EPokedexNo> GetImplementedSpeciesNo()
	{
		return ImplementedSpeciesNo;
	}

	static std::string GetSpeciesName(EPokedexNo _Id)
	{
		return Species[_Id].Name;
	}

protected:

private:
	// NameResolver
	static std::map<std::string, int> NameResolver;
	static int Resolve(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);
		
		if (false == NameResolver.contains(UpperName))
		{
			MsgBoxAssert("UPokemonDB::NameResolver에서 에러가 발생했습니다.");
			return -1;
		}

		return NameResolver[UpperName];
	}

	// 포켓몬
	static std::map<EPokedexNo, FPokemonSpecies> Species;
	static std::map<EPokemonMove, FPokemonMove> Moves;
	static std::map<EPokemonNature, FPokemonNature> Natures;
	static std::map<EPokemonAbility, FPokemonAbility> Abilities;
	static std::map<EPokemonGender, FPokemonGender> Genders;
	static std::map<EPokemonStatus, FPokemonStatus> Status;
	static std::map<EPokemonType, FPokemonType> Types;
	static std::list<EPokedexNo> ImplementedSpeciesNo;

	// Zones[맵 이름][번호] = (Zone 객체)
	static std::map<std::string, std::map<int, UWildPokemonZone>> WildPokemonZones;

	// constructor destructor
	UPokemonDB() {}
	~UPokemonDB() {}
};

