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

	// 엔트리
	static UPokemon& GetPokemonInEntry(int _Index);
	static void AddPokemonToEntry(const UPokemon& _Pokemon);
	static void AddPokemonToComputer(const UPokemon& _Pokemon);
	static void SwapEntry(int _Index0, int _Index1);
	static int GetPokemonEntrySize();

	// 게임 진행도
	static bool IsAchieved(EAchievement _Achievement);
	static void Achieve(EAchievement _Achievement);

	// 도감
	static void SeePokemon(EPokemonId _PokedexNo);
	static void OwnPokemon(EPokemonId _PokedexNo);
	static int GetSeenPokemonCount();
	static int GetOwnedPokemonCount();

	// 플레이어 정보
	static int GetIdNo();
	static std::wstring GetIdNoW();
	static std::wstring GetNickNameW();
	static int GetMoney();
	static void LoseMoney(int _Money);
	static void GainMoney(int _Money);

	// 인벤토리
	static void GainItem(EItemId _Id, int _Count = 1);
	static void LoseItem(EItemId _Id, int _Count = 1);
	static const std::map<EItemId, int>* GetUseItemsReadonly();
	static const std::map<EItemId, int>* GetPokeBallsReadonly();
protected:

private:
	// 유저 정보
	static int IdNo;
	static std::string Nickname;
	static int Money;
	
	// 포켓몬 정보
	static std::vector<UPokemon> PokemonEntry;
	static std::vector<UPokemon> PokemonInComputer;

	// 도감 정보
	static std::map<EPokemonId, bool> PokedexSeenMap;
	static std::map<EPokemonId, bool> PokedexOwnedMap;

	// 업적 정보 (이벤트 발생 조건으로 사용)
	static std::map<EAchievement, bool> AchievementMap;

	// 인벤토리
	static std::map<EItemType, std::map<EItemId, int>> Inventory;
};

