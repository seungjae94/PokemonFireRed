#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include <EngineBase/EngineDebug.h>
#include "PokemonString.h"
#include "Global.h"
#include "PokemonDB.h"

class UPokemon
{
public:
	// constructor destructor
	UPokemon() {};
	UPokemon(EPokedexNo _Id, int _Level = 1);
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
		return Species->Id;
	}
	std::wstring GetPokedexNoW() const
	{
		return UPokemonString::PadLeft(std::to_wstring(static_cast<int>(GetPokedexNo())), 3, L'0');
	}

	std::string GetSpeciesNameA() const
	{
		return Species->Name;
	}

	std::wstring GetSpeciesNameW() const
	{
		return UEngineString::AnsiToUniCode(Species->Name);
	}

	std::wstring GetNatureNameW() const
	{
		return UEngineString::AnsiToUniCode(Nature->Name);
	}

	std::string GetAbilityNameA() const
	{
		return Ability->Name;
	}

	std::wstring GetAbilityNameW() const
	{
		return UEngineString::AnsiToUniCode(Ability->Name);
	}

	std::wstring GetAbilityUpperW() const
	{
		std::string UpperA = UEngineString::ToUpper(GetAbilityNameA());
		return UEngineString::AnsiToUniCode(UpperA);
	}

	std::wstring GetAbilityExplainW() const
	{
		return UEngineString::AnsiToUniCode(Ability->Explain);
	}

	std::string GetGenderImageName() const
	{
		return Gender->ImageName;
	}

	std::string GetBigGenderImageName() const
	{
		return Gender->BigImageName;
	}

	int GetTypeCount() const
	{
		return static_cast<int>(Species->TypeIds.size());
	}

	std::string GetTypeImageName(int _Index) const;

	int GetMoveCount() const
	{
		return static_cast<int>(Moves.size());
	}

	std::wstring GetMoveNameW(int _Index) const;
	std::wstring GetMovePPW(int _Index) const;
	std::wstring GetMoveCurPPW(int _Index) const;
	std::string GetMoveTypeImageName(int _Index) const;
	
	int GetMovePower(int _Index) const
	{
		return Moves[_Index]->BasePower;
	}
	std::wstring GetMovePowerW(int _Index) const;
	std::wstring GetMoveAccuracyW(int _Index) const;
	std::wstring GetMoveExplainW(int _Index) const;

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

private:
	// 기본 정보
	std::wstring Name;
	const FPokemonGender* Gender = nullptr;
	int Level = 0;
	int AccExp = 0;
	int CurHp = 0;

	// 종족 정보
	const FPokemonSpecies* Species = nullptr;

	// 전투 관련 정보
	const FPokemonNature* Nature = nullptr;
	const FPokemonAbility* Ability = nullptr;
	std::vector<const FPokemonMove*> Moves;
	std::vector<int> MoveCurPPs;

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

	// 함수
	int GetAccExpForLevel(int _Level) const;
	int GetErraticAccExpForLevel(int _Level) const;
	int GetFluctuatingAccExpForLevel(int _Level) const;
	void InitMoves();
	void InitRandomIVs();
	void InitRandomGender();
	void InitRandomNature();
	void InitRandomAbility();
};

