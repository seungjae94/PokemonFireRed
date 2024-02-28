#include "PlayerData.h"
#include "PokemonMath.h"

int UPlayerData::IdNo = -1;
std::string UPlayerData::Nickname = "RED";
int UPlayerData::Money = 3000;
std::vector<UPokemon> UPlayerData::PokemonEntry;
std::vector<UPokemon> UPlayerData::PokemonInComputer;
std::map<EAchievement, bool> UPlayerData::AchievementMap;
std::map<EPokedexNo, bool> UPlayerData::PokedexSeenMap;
std::map<EPokedexNo, bool> UPlayerData::PokedexOwnedMap;

UPokemon UPlayerData::EnemyWildPokemon;

UPlayerData::UPlayerData()
{
}

UPlayerData::~UPlayerData()
{
}

UPokemon& UPlayerData::GetPokemonInEntry(int _Index)
{
	return PokemonEntry[_Index];
}

void UPlayerData::AddPokemonToEntry(const UPokemon& _Pokemon)
{
	if (PokemonEntry.size() == 6)
	{
		AddPokemonToComputer(_Pokemon);
		return;
	}

	OwnPokemon(_Pokemon.GetPokedexNo());
	PokemonEntry.push_back(_Pokemon);
}

void UPlayerData::AddPokemonToComputer(const UPokemon& _Pokemon)
{
	PokemonInComputer.push_back(_Pokemon);
}

void UPlayerData::SwapEntry(int _Index0, int _Index1)
{
	if (_Index0 < 0 || _Index0 >= PokemonEntry.size())
	{
		MsgBoxAssert("Index0: " + std::to_string(_Index0) + "는 엔트리 인덱스를 벗어났습니다.");
		return;
	}

	if (_Index1 < 0 || _Index1 >= PokemonEntry.size())
	{
		MsgBoxAssert("Index1: " + std::to_string(_Index1) + "는 엔트리 인덱스를 벗어났습니다.");
		return;
	}

	UPokemon Pokemon0 = PokemonEntry[_Index0];
	UPokemon Pokemon1 = PokemonEntry[_Index1];
	PokemonEntry[_Index0] = Pokemon1;
	PokemonEntry[_Index1] = Pokemon0;
}

int UPlayerData::GetIdNo()
{
	if (IdNo == -1)
	{
		// 아직 IdNo 값이 할당되지 않은 경우 랜덤하게 값을 할당해서 반환한다.
		IdNo = UPokemonMath::RandomInt(0, 65535);
	}

	return IdNo;
}



