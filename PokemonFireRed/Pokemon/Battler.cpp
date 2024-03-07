#include "Battler.h"
#include "PlayerData.h"
#include "EventManager.h"

UBattler::UBattler()
{
}

UBattler::~UBattler()
{
}

void UBattler::InitPlayer()
{
	IsPlayerValue = true;

	int PlayerEntrySize = static_cast<int>(UPlayerData::GetPokemonEntrySize());
	for (int i = 0; i < PlayerEntrySize; ++i)
	{
		Entry.push_back(&UPlayerData::GetPokemonInEntry(i));
	}
}

void UBattler::InitWildPokemon()
{
	IsPlayerValue = false;
	IsWildPokemonValue = true;
	InitEnemyPokemon();
}

void UBattler::InitTrainer()
{
	IsPlayerValue = false;
	IsWildPokemonValue = false;
	InitEnemyPokemon();
}

void UBattler::InitEnemyPokemon()
{
	std::vector<UPokemon>* RealEntry = UEventManager::LoadEnemyEntry();

	if (nullptr == RealEntry)
	{
		MsgBoxAssert("�� ���ϸ��� ���� ���·� ��Ʋ�� �����Ϸ��� �մϴ�.");
		return;
	}

	Entry.clear();
	for (int i = 0; i < RealEntry->size(); ++i)
	{
		Entry.push_back(&(RealEntry->at(i)));
	}
}
