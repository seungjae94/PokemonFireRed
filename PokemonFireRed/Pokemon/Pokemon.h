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
		return Species->PokedexNo;
	}
	std::wstring GetPokedexNoW() const
	{
		return UPokemonString::PadLeft(std::to_wstring(static_cast<int>(GetPokedexNo())), 3, L'0');
	}

	std::string GetSpeciesNameA() const
	{
		return Species->SpeciesName;
	}

	std::wstring GetSpeciesNameW() const
	{
		return UEngineString::AnsiToUniCode(Species->SpeciesName);
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

	std::vector<std::string> GetTypeImageNames(bool _PlaceHolder = true) const;

	int GetMoveCount() const
	{
		return static_cast<int>(Moves.size());
	}

	std::wstring GetMoveNameW(int _Index) const
	{
		if (_Index < 0 || _Index >= 4)
		{
			MsgBoxAssert("�ε��� " + std::to_string(_Index) + "�� �߸� �Ǿ� Move �̸��� ã�� �� �����ϴ�.");
			return L"";
		}

		if (_Index < Moves.size())
		{
			return Moves[_Index]->GetNameW();
		}

		return L"-";
	}

	std::wstring GetMovePPW(int _Index) const
	{
		if (_Index < 0 || _Index >= 4)
		{
			MsgBoxAssert("�ε��� " + std::to_string(_Index) + "�� �߸� �Ǿ� Move �̸��� ã�� �� �����ϴ�.");
			return L"";
		}

		if (_Index < Moves.size())
		{
			return std::to_wstring(Moves[_Index]->PP);
		}

		return L"--";
	}

	std::string GetMoveTypeImageName(int _Index) const
	{
		if (_Index < 0 || _Index >= 4)
		{
			MsgBoxAssert("�ε��� " + std::to_string(_Index) + "�� �߸� �Ǿ� Move �̸��� ã�� �� �����ϴ�.");
			return "";
		}

		if (_Index < Moves.size())
		{
			const FPokemonType* Type = UPokemonDB::FindType(Moves[_Index]->Type);
			return Type->ImageName;
		}

		return RN::TypePlaceHolder;
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
	
	// (����ġ ������ ����) = (���� ������ ���� ����ġ) - (���� ������ ���� ����ġ)
	int GetExpSize() const;

	// (���� �������� �ʿ��� ����ġ) = (���� ������ ���� ����ġ) - (���� ����ġ)
	int GetNextLevelExp() const;
	std::wstring GetNextLevelExpW() const
	{
		return std::to_wstring(GetNextLevelExp());
	}

	// (���� �������� ���� ����ġ) = (����ġ ������ ����) - (���� �������� �ʿ��� ����ġ)
	int GetExp() const;

	void SetLevel(int _Level)
	{
		Level = _Level;
	}

protected:

private:
	// �⺻ ����
	std::wstring Name;
	const FPokemonGender* Gender = nullptr;
	int Level = 0;
	int AccExp = 0;
	int CurHp = 0;

	// ���� ����
	const FPokemonSpecies* Species = nullptr;

	// ���� ���� ����
	const FPokemonNature* Nature = nullptr;
	const FPokemonAbility* Ability = nullptr;
	std::vector<const FPokemonMove*> Moves;

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

	// �Լ�
	int GetAccExpForLevel(int _Level) const;
	int GetErraticAccExpForLevel(int _Level) const;
	int GetFluctuatingAccExpForLevel(int _Level) const;
	void InitMoves();
	void InitRandomIVs();
	void InitRandomGender();
	void InitRandomNature();
	void InitRandomAbility();
};

