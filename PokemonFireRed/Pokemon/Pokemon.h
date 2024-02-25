#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include "Global.h"
#include "PokemonData.h"

class UPokemon
{
public:
	// constructor destructor
	UPokemon();
	~UPokemon();

	// delete Function
	UPokemon(const UPokemon& _Other) = default;
	UPokemon(UPokemon&& _Other) noexcept = default;
	UPokemon& operator=(const UPokemon& _Other) = default;
	UPokemon& operator=(UPokemon&& _Other) noexcept = default;

	std::wstring GetNameW() const
	{
		return Name;
	}

	int GetLevel() const
	{
		return Level;
	}
	std::wstring GetLevelW() const
	{
		return std::to_wstring(GetLevel());
	}

	int GetCurHp() const
	{
		return CurHp;
	}

	std::wstring GetCurHpW() const
	{
		return std::to_wstring(GetCurHp());
	}

	void SetCurHp(int _CurHp)
	{
		CurHp = _CurHp;
	}

	EPokedexNo GetPokedexNo() const
	{
		return PokedexNo;
	}

	std::string GetSpeciesA() const
	{
		return UPokemonData::GetSpeciesName(PokedexNo);
	}

	std::wstring GetSpeciesW() const
	{
		return UPokemonData::GetSpeciesNameW(PokedexNo);
	}

	ENature GetNature() const
	{
		return Nature;
	}

	std::wstring GetNatureW() const
	{
		return UPokemonData::GetNatureNameW(Nature);
	}

	EAbility GetAbility() const
	{
		return Ability;
	}

	std::string GetAbilityA() const
	{
		return UPokemonData::GetAbilityName(Ability);
	}

	std::wstring GetAbilityW() const
	{
		return UPokemonData::GetAbilityNameW(Ability);
	}

	std::wstring GetAbilityUpperW() const
	{
		std::string UpperA = UEngineString::ToUpper(GetAbilityA());
		return UEngineString::AnsiToUniCode(UpperA);
	}

	std::wstring GetAbilityExplainW() const
	{
		return UPokemonData::GetAbilityExplainTextW(Ability);
	}

	EGender GetGender() const
	{
		return Gender;
	}

	std::string GetGenderImageName() const
	{
		return UPokemonData::GetGenderImageName(Gender);
	}

	std::string GetBigGenderImageName() const
	{
		return UPokemonData::GetBigGenderImageName(Gender);
	}

	std::vector<EPokemonType> GetTypes() const
	{
		return Types;
	}

	std::vector<std::string> GetTypeImageNames(bool _PlaceHolder = true) const
	{
		std::vector<std::string> Result;

		int TypeSize = static_cast<int>(Types.size());

		for (int i = 0; i < TypeSize; ++i)
		{
			std::string ImageName = UPokemonData::GetTypeImageName(Types[i]);
			Result.push_back(ImageName);
		}
		for (int i = TypeSize; i < 2; ++i)
		{
			Result.push_back(RN::TypePlaceHolder);
		}

		return Result;
	}

	void Heal(int _Value);

	void HealAll()
	{
		CurHp = GetHp();
	}

	int GetHp() const;
	std::wstring GetHpW() const
	{
		return std::to_wstring(GetHp());
	}

	int GetAtk() const;
	std::wstring GetAtkW() const
	{
		return std::to_wstring(GetAtk());
	}

	int GetDef() const;
	std::wstring GetDefW() const
	{
		return std::to_wstring(GetDef());
	}

	int GetSpAtk() const;
	std::wstring GetSpAtkW() const
	{
		return std::to_wstring(GetSpAtk());
	}

	int GetSpDef() const;
	std::wstring GetSpDefW() const
	{
		return std::to_wstring(GetSpDef());
	}

	int GetSpeed() const;
	std::wstring GetSpeedW() const
	{
		return std::to_wstring(GetSpeed());
	}

	void GainExp(int _Exp);

	int GetAccExp() const
	{
		return AccExp;
	}
	std::wstring GetAccExpW() const
	{
		return std::to_wstring(GetAccExp());
	}
	
	// (경험치 구간의 길이) = (다음 레벨의 누적 경험치) - (현재 레벨의 누적 경험치)
	int GetExpSize() const;

	// (다음 레벨까지 필요한 경험치) = (다음 레벨의 누적 경험치) - (누적 경험치)
	int GetNextLevelExp() const;
	std::wstring GetNextLevelExpW() const
	{
		return std::to_wstring(GetNextLevelExp());
	}

	// (현재 레벨에서 얻은 경험치) = (경험치 구간의 길이) - (다음 레벨까지 필요한 경험치)
	int GetExp() const;

	void SetLevel(int _Level)
	{
		Level = _Level;
	}

protected:
	// Base Stat
	int BHp = 0;
	int BAtk = 0;
	int BDef = 0;
	int BSpAtk = 0;
	int BSpDef = 0;
	int BSpeed = 0;

	// Effort Value Yield (기절했을 때 상대에게 주는 노력치)
	int YHp = 0;
	int YAtk = 0;
	int YDef = 0;
	int YSpAtk = 0;
	int YSpDef = 0;
	int YSpeed = 0;

	// 기본 정보
	std::wstring Name;
	EPokedexNo PokedexNo = EPokedexNo::None;
	int Level = 0;
	int AccExp = 0;
	int CurHp = 0;

	// 상세 정보
	EAbility Ability = EAbility::NONE;
	EExperienceGroup ExpGroup = EExperienceGroup::NONE;
	std::vector<EPokemonType> Types;
	ENature Nature = ENature::NONE;
	int CatchRate = 0;
	int Friendship = 0;

	// 함수
	void SetRandomIVs();
	void SetRandomGender(float _MaleRatio);
	void SetRandomNature();
	void SetRandomAbility(std::vector<EAbility> _Abilities);
	int GetAccExpForLevel(int _Level) const;
	int GetErraticAccExpForLevel(int _Level) const;
	int GetFluctuatingAccExpForLevel(int _Level) const;

private:
	// Individual Value
	int IHp = 0;
	int IAtk = 0;
	int IDef = 0;
	int ISpAtk = 0;
	int ISpDef = 0;
	int ISpeed = 0;

	// Effort Value
	int EHp = 0;
	int EAtk = 0;
	int EDef = 0;
	int ESpAtk = 0;
	int ESpDef = 0;
	int ESpeed = 0;

	// 기타
	EGender Gender = EGender::Male;


};

