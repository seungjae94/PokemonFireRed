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
	SelectFirstPokemon,
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

	// ��Ʈ��
	static UPokemon& GetPokemonInEntry(int _Index);
	static void AddPokemonToEntry(const UPokemon& _Pokemon);
	static void AddPokemonToComputer(const UPokemon& _Pokemon);
	static void SwapEntry(int _Index0, int _Index1);
	static int GetPokemonEntrySize();

	// ���� ���൵
	static bool IsAchieved(EAchievement _Achievement);
	static void Achieve(EAchievement _Achievement);

	// ����
	static void SeePokemon(EPokemonId _PokedexNo);
	static void OwnPokemon(EPokemonId _PokedexNo);
	static int GetSeenPokemonCount();
	static int GetOwnedPokemonCount();

	// �÷��̾� ����
	static int GetIdNo();
	static std::wstring GetIdNoW();
	static std::wstring GetNickNameW();
	static int GetMoney();
	static void LoseMoney(int _Money);
	static void GainMoney(int _Money);

	// �κ��丮
	static void GainItem(EItemId _Id, int _Count = 1);
	static void LoseItem(EItemId _Id, int _Count = 1);
	static const std::map<EItemId, int>* GetUseItemsReadonly();
	static const std::map<EItemId, int>* GetPokeBallsReadonly();
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
	static std::map<EPokemonId, bool> PokedexSeenMap;
	static std::map<EPokemonId, bool> PokedexOwnedMap;

	// ���� ���� (�̺�Ʈ �߻� �������� ���)
	static std::map<EAchievement, bool> AchievementMap;

	// �κ��丮
	static std::map<EItemType, std::map<EItemId, int>> Inventory;
};

