#pragma once
#include <map>
#include <vector>
#include <list>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include "PokemonString.h"
#include "Global.h"
#include "WildPokemonZone.h"
#include "Item.h"

class UGameData
{
	friend class GameDataInitiator;
public:
	// delete Function
	UGameData(const UGameData& _Other) = delete;
	UGameData(UGameData&& _Other) noexcept = delete;
	UGameData& operator=(const UGameData& _Other) = delete;
	UGameData& operator=(UGameData&& _Other) noexcept = delete;

	static const FPokemonSpecies* FindSpecies(EPokemonId _Id);
	static const FPokemonMove* FindMove(EPokemonMove _Id);
	static const FPokemonNature* FindNature(EPokemonNature _Id);
	static const FPokemonAbility* FindAbility(EPokemonAbility _Id);
	static const FPokemonGender* FindGender(EPokemonGender _Id);
	static const FPokemonType* FindType(EPokemonType _Id);
	static const FPokemonStatus* FindStatus(EPokemonStatus _Id);
	static const UWildPokemonZone* FindWildPokemonZone(std::string_view _MapName, int _Index);
	static const FItem* FindItem(EItemId _Id);

	static std::list<EPokemonId> GetImplementedSpeciesNo();
	static std::string GetSpeciesName(EPokemonId _Id);

protected:

private:
	// NameResolver
	static std::map<std::string, int> NameResolver;
	static int Resolve(std::string_view _Name);

	// * -> acute e
	static std::wstring AnsiToUnicodeWithReplacement(std::string _Text);

	// 포켓몬
	static std::map<EPokemonId, FPokemonSpecies> Species;
	static std::map<EPokemonMove, FPokemonMove> Moves;
	static std::map<EPokemonNature, FPokemonNature> Natures;
	static std::map<EPokemonAbility, FPokemonAbility> Abilities;
	static std::map<EPokemonGender, FPokemonGender> Genders;
	static std::map<EPokemonStatus, FPokemonStatus> Statuses;
	static std::map<EPokemonType, FPokemonType> Types;
	static std::list<EPokemonId> ImplementedSpeciesNo;

	// Zones[맵 이름][번호] = (Zone 객체)
	static std::map<std::string, std::map<int, UWildPokemonZone>> WildPokemonZones;

	// 아이템
	static std::map<EItemId, FItem> Items;

	// constructor destructor
	UGameData() {}
	~UGameData() {}
};

