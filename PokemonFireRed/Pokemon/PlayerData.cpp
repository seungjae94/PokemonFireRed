#include "PlayerData.h"
#include "PokemonMath.h"

int UPlayerData::IdNo = -1;
std::string UPlayerData::Nickname = "RED";
int UPlayerData::Money = 3000;
std::vector<UPokemon> UPlayerData::PokemonEntry;
std::vector<UPokemon> UPlayerData::PokemonInComputer;
std::map<EAchievement, bool> UPlayerData::AchievementMap;
std::map<EPokemonId, bool> UPlayerData::PokedexSeenMap;
std::map<EPokemonId, bool> UPlayerData::PokedexOwnedMap;
std::map<EItemType, std::map<EItemId, int>> UPlayerData::Inventory;

class PlayerDataReleaser
{
public:
	PlayerDataReleaser(){}
	~PlayerDataReleaser(){
	}
};
PlayerDataReleaser Releaser;

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
		MsgBoxAssert("Index0: " + std::to_string(_Index0) + "�� ��Ʈ�� �ε����� ������ϴ�.");
		return;
	}

	if (_Index1 < 0 || _Index1 >= PokemonEntry.size())
	{
		MsgBoxAssert("Index1: " + std::to_string(_Index1) + "�� ��Ʈ�� �ε����� ������ϴ�.");
		return;
	}

	UPokemon Pokemon0 = PokemonEntry[_Index0];
	UPokemon Pokemon1 = PokemonEntry[_Index1];
	PokemonEntry[_Index0] = Pokemon1;
	PokemonEntry[_Index1] = Pokemon0;
}

int UPlayerData::GetPokemonEntrySize()
{
	return static_cast<int>(PokemonEntry.size());
}

bool UPlayerData::IsAchieved(EAchievement _Achievement)
{
	if (false == AchievementMap.contains(_Achievement))
	{
		AchievementMap[_Achievement] = false;
		return false;
	}

	return AchievementMap[_Achievement];
}

void UPlayerData::Achieve(EAchievement _Achievement)
{
	AchievementMap[_Achievement] = true;
}

void UPlayerData::SeePokemon(EPokemonId _PokedexNo)
{
	PokedexSeenMap[_PokedexNo] = true;
}

void UPlayerData::OwnPokemon(EPokemonId _PokedexNo)
{
	PokedexSeenMap[_PokedexNo] = true;
	PokedexOwnedMap[_PokedexNo] = true;
}

int UPlayerData::GetSeenPokemonCount()
{
	return static_cast<int>(PokedexSeenMap.size());
}

int UPlayerData::GetOwnedPokemonCount()
{
	return static_cast<int>(PokedexOwnedMap.size());
}

int UPlayerData::GetIdNo()
{
	if (IdNo == -1)
	{
		// ���� IdNo ���� �Ҵ���� ���� ��� �����ϰ� ���� �Ҵ��ؼ� ��ȯ�Ѵ�.
		IdNo = UPokemonMath::RandomInt(0, 65535);
	}

	return IdNo;
}

std::wstring UPlayerData::GetIdNoW()
{
	return UPokemonString::PadLeft(std::to_wstring(GetIdNo()), 5, L'0');
}

std::wstring UPlayerData::GetNickNameW()
{
	return UEngineString::AnsiToUniCode(Nickname);
}

int UPlayerData::GetMoney()
{
	return Money;
}

void UPlayerData::LoseMoney(int _Money)
{
	if (_Money <= 0)
	{
		MsgBoxAssert("[UPlayerData::LoseMoney] 0 ������ ���� ���� �� �����ϴ�.");
		return;
	}

	Money -= _Money;

	if (Money < 0)
	{
		Money = 0;
	}
}

void UPlayerData::GainMoney(int _Money)
{
	if (_Money <= 0)
	{
		MsgBoxAssert("[UPlayerData::GainMoney] 0 ������ ���� ���� �� �����ϴ�.");
		return;
	}

	Money += _Money;
}

void UPlayerData::GainItem(EItemId _Id, int _Count)
{
	const FItem* Item = UGameDB::FindItem(_Id);
	EItemType ItemType = Item->Type;

	if (false == Inventory.contains(ItemType) || false == Inventory[ItemType].contains(_Id))
	{
		Inventory[ItemType][_Id] = 0;
	}

	Inventory[ItemType][_Id] += _Count;
}

void UPlayerData::LoseItem(EItemId _Id, int _Count)
{
	const FItem* Item = UGameDB::FindItem(_Id);
	EItemType ItemType = Item->Type;

	if (false == Inventory.contains(ItemType) || false == Inventory[ItemType].contains(_Id))
	{
		MsgBoxAssert("[UPlayerData::LoseItem] ���� ���� �ʴ� �������� ���� �� �����ϴ�.");
		return;
	}

	int HaveCount = Inventory[ItemType][_Id];

	if (_Count > HaveCount)
	{
		MsgBoxAssert("[UPlayerData::LoseItem] Id: " + std::to_string(static_cast<int>(_Id)) + " �������� " 
			+ std::to_string(HaveCount) + "���� ���� �����Ƿ� " + std::to_string(_Count) + "���� ���� �� �����ϴ�.");
		return;
	}

	Inventory[ItemType][_Id] -= _Count;

	if (Inventory[ItemType][_Id] <= 0)
	{
		Inventory[ItemType].erase(_Id);
	}
}

std::list<FInventoryRecord> UPlayerData::GetItemList(EItemType _ItemType, int _StartIndex, int _EndIndex)
{
	const std::map<EItemId, int>& Map = Inventory[_ItemType];

	if (_StartIndex > _EndIndex)
	{
		MsgBoxAssert("[UPlayerData::GetItemList] �ε����� �߸� �����߽��ϴ�. ���� �ε����� ���� �ε������� Ů�ϴ�.");
		return {};
	}

	if (_StartIndex < 0)
	{
		MsgBoxAssert("[UPlayerData::GetItemList] �ε����� �߸� �����߽��ϴ�. ���� �ε����� 0���� �۽��ϴ�.");
		return {};
	}

	if (_EndIndex >= Map.size())
	{
		MsgBoxAssert("[UPlayerData::GetItemList] �ε����� �߸� �����߽��ϴ�. ���� �ε����� ���� ũ�� �̻��Դϴ�.");
		return {};
	}

	std::map<EItemId, int>::const_iterator StartIter = Map.begin();
	StartIter = std::next(StartIter, _StartIndex);

	std::list<FInventoryRecord> Result;

	for (int i = 0; i <= _EndIndex - _StartIndex; ++i)
	{
		FInventoryRecord Record;
		Record.Id = StartIter->first;
		Record.Count = StartIter->second;
		Result.push_back(Record);

		++StartIter;
	}

	return Result;
}

const FItem* UPlayerData::GetItem(EItemType _ItemType, int _Index)
{
	const std::map<EItemId, int>& Map = Inventory[_ItemType];

	if (_Index >= Map.size())
	{
		MsgBoxAssert("[UPlayerData::GetItem] �ε����� �߸� �����߽��ϴ�. �ε����� ���� ũ�� �̻��Դϴ�.");
		return {};
	}

	std::map<EItemId, int>::const_iterator StartIter = Map.begin();
	StartIter = std::next(StartIter, _Index);

	EItemId Id = StartIter->first;

	return UGameDB::FindItem(Id);
}

int UPlayerData::GetRecordCount(EItemType _ItemType)
{
	return static_cast<int>(Inventory[_ItemType].size());
}

