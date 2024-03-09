#pragma once
#include <vector>
#include <map>
#include "Pokemon.h"
#include "PokemonString.h"

enum class EPokedexState
{

};

enum class EAchievement
{
	None,
	GetStarterEventStart,
	GetFirstPokemon,
	FightWithGreen,
	GetPokedex
};

class UPlayerData
{
	friend class PlayerDataReleaser;
public:
	// constructor destructor
	UPlayerData();
	~UPlayerData();

	static UPokemon& GetPokemonInEntry(int _Index);
	static void AddPokemonToEntry(const UPokemon& _Pokemon);
	static void AddPokemonToComputer(const UPokemon& _Pokemon);
	
	static void SwapEntry(int _Index0, int _Index1);

	static int GetPokemonEntrySize()
	{
		return static_cast<int>(PokemonEntry.size());
	}

	static bool IsAchieved(EAchievement _Achievement)
	{
		return AchievementMap[_Achievement];
	}

	static void Achieve(EAchievement _Achievement)
	{
		AchievementMap[_Achievement] = true;
	}

	static void SeePokemon(EPokedexNo _PokedexNo)
	{
		PokedexSeenMap[_PokedexNo] = true;
	}

	static void OwnPokemon(EPokedexNo _PokedexNo)
	{
		PokedexSeenMap[_PokedexNo] = true;
		PokedexOwnedMap[_PokedexNo] = true;
	}

	static int GetSeenPokemonCount()
	{
		return static_cast<int>(PokedexSeenMap.size());
	}

	static int GetOwnedPokemonCount()
	{
		return static_cast<int>(PokedexOwnedMap.size());
	}

	static int GetIdNo();
	static std::wstring GetIdNoW()
	{
		return UPokemonString::PadLeft(std::to_wstring(GetIdNo()), 5, L'0');
	}

	static std::wstring GetNickNameW()
	{
		return UEngineString::AnsiToUniCode(Nickname);
	}

	static int GetMoney()
	{
		return Money;
	}

	static void LostMoney(int _Money)	
	{
		if (_Money <= 0)
		{
			MsgBoxAssert("[UPlayerData::LostMoney] 0 ������ ���� ���� �� �����ϴ�.");
			return;
		}
	}

	static void GainMoney(int _Money)
	{
		if (_Money <= 0)
		{
			MsgBoxAssert("[UPlayerData::GainMoney] 0 ������ ���� ���� �� �����ϴ�.");
			return;
		}
	}

protected:

private:
	// ���� ����
	static int IdNo;
	static std::string Nickname;
	static int Money;
	
	// ���ϸ� ����
	static std::vector<UPokemon> PokemonEntry;
	static std::vector<UPokemon> PokemonInComputer;

	// ���� ����
	static std::map<EPokedexNo, bool> PokedexSeenMap;
	static std::map<EPokedexNo, bool> PokedexOwnedMap;

	// ���� ���� (�̺�Ʈ �߻� �������� ���)
	static std::map<EAchievement, bool> AchievementMap;
};

