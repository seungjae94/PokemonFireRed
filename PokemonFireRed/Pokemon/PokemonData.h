#pragma once
#include <map>
#include <list>
#include <EngineBase/EngineString.h>
#include "Global.h"

enum class EPokedexNo
{
	None,
	Bulbasaur = 1,
	Charmander = 4,
	Squirtle = 7,
};

enum class EAbility
{
	NONE,
	Overgrow,
	Blaze,
	Torrent,
	ShieldDust,
	ShedSkin,
	CompoundEyes,
	Swarm,
	KeenEye,
	RunAway,
	Guts,
	Intimidate,
	Static,
	SandVeil,
	PoisonPoint,
};

enum class EGender
{
	Male,
	Female
};

enum class ENature
{
	NONE,		// Inc (x1.1)	Dec (x0.9)
	Hardy,		// None			None
	Lonely,		// Atk			Def
	Brave,		// Atk			Speed
	Adamant,	// Atk			SpAtk
	Naughty,	// Atk			SpDef
	Docile,		// None			None
	Bold,		// Def			Atk
	Relaxed,	// Def			Speed
	Impish,		// Def			SpAtk
	Lax,		// Def			SpDef
	Serious,	// None			None
	Timid,		// Speed		Atk
	Hasty,		// Speed		Def
	Jolly,		// Speed		SpAtk
	Naive,		// Speed		SpDef
	Bashful,	// None			None
	Modest,		// SpAtk		Atk
	Mild,		// SpAtk		Def
	Quiet,		// SpAtk		Speed
	Rash,		// SpAtk		SpDef
	Quirky,		// None			None
	Calm,		// SpDef		Atk
	Gentle,		// SpDef		Def
	Sassy,		// SpDef		Speed
	Careful,	// SpDef		SpAtk
	MAX,
};

enum class EPokemonStatus
{
	Normal,
	Sleep,
	Poison,
	Burn,
	Freeze,
	Paralysis,
};

enum class EExperienceGroup
{
	NONE,		// 레벨 100 기준 누적 경험치		누적 경험치 공식
	Erratic,	// 600,000						piecewise
	Fast,		// 800,000						0.8n^3
	MediumFast,	// 1,000,000					n^3
	MediumSlow,	// 1,059,860					1.2n^3 - 15n^2 + 100n - 140
	Slow,		// 1,250,000					1.25n^3
	Fluctuating	// 1,640,000					piecewise
};

enum class EPokemonType
{
	Normal,
	Fighting,
	Flying,
	Poison,
	Ground,
	Rock,
	Bug,
	Ghost,
	Steel,
	Fire,
	Water,
	Grass,
	Electric,
	Psychic,
	Ice,
	Dragon,
	Dark,
	Fairy
};


class UPokemonData
{
	friend class PokemonDataInitiator;
public:
	// delete Function
	UPokemonData(const UPokemonData& _Other) = delete;
	UPokemonData(UPokemonData&& _Other) noexcept = delete;
	UPokemonData& operator=(const UPokemonData& _Other) = delete;
	UPokemonData& operator=(UPokemonData&& _Other) noexcept = delete;

	// 유틸 함수
	static std::wstring GetSpeciesNameW(EPokedexNo _No)
	{
		return UEngineString::AnsiToUniCode(SpeciesNames[_No]);
	}

	static std::string GetSpeciesName(EPokedexNo _No)
	{
		return SpeciesNames[_No];
	}

	static std::wstring GetNatureNameW(ENature _Nature)
	{
		return UEngineString::AnsiToUniCode(NatureNames[_Nature]);
	}

	static std::string GetNatureName(ENature _Nature)
	{
		return NatureNames[_Nature];
	}

	static std::wstring GetAbilityNameW(EAbility _Ability)
	{
		return UEngineString::AnsiToUniCode(GetAbilityName(_Ability));
	}


	static std::string GetAbilityName(EAbility _Ability)
	{
		return AbilityNames[_Ability];
	}

	static std::wstring GetAbilityExplainTextW(EAbility _Ability)
	{
		return UEngineString::AnsiToUniCode(GetAbilityExplainText(_Ability));
	}

	static std::string GetAbilityExplainText(EAbility _Ability)
	{
		return AbilityExplainTexts[_Ability];
	}

	static std::string GetTypeImageName(EPokemonType _Type)
	{
		return TypeImageNames[_Type];
	}

	static std::string GetGenderImageName(EGender _Gender)
	{
		if (_Gender == EGender::Male)
		{
			return RN::GenderMarkMale;
		}
		else
		{
			return RN::GenderMarkFemale;
		}
	}

	static std::string GetBigGenderImageName(EGender _Gender)
	{
		if (_Gender == EGender::Male)
		{
			return RN::BigGenderMarkMale;
		}
		else
		{
			return RN::BigGenderMarkFemale;
		}
	}

	static std::list<EPokedexNo> GetImplementedSpeciesNo()
	{
		return ImplementedSpeciesNo;
	}

protected:

private:
	static std::map<EPokedexNo, std::string> SpeciesNames;
	static std::map<ENature, std::string> NatureNames;
	static std::map<EPokemonType, std::string> TypeImageNames;
	static std::map<EAbility, std::string> AbilityNames;
	static std::map<EAbility, std::string> AbilityExplainTexts;
	static std::list<EPokedexNo> ImplementedSpeciesNo;

	// constructor destructor
	UPokemonData() {}
	~UPokemonData() {}
};

