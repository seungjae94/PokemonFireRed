#include "UserData.h"
#include "PokemonMath.h"

int UUserData::IdNo = -1;
std::string UUserData::Nickname = "RED";
int UUserData::Money = 3000;
std::vector<UPokemon> UUserData::PokemonEntry;
std::vector<UPokemon> UUserData::PokemonInComputer;
std::map<EAchievement, bool> UUserData::AchievementMap;
std::map<EPokemonId, bool> UUserData::PokedexSeenMap;
std::map<EPokemonId, bool> UUserData::PokedexOwnedMap;
std::map<EItemType, std::map<EItemId, int>> UUserData::Inventory;

class UserDataReleaser
{
public:
	UserDataReleaser(){}
	~UserDataReleaser(){
	}
};
UserDataReleaser Releaser;

UUserData::UUserData()
{
}

UUserData::~UUserData()
{
}

UPokemon& UUserData::GetPokemonInEntry(int _Index)
{
	return PokemonEntry[_Index];
}

void UUserData::AddPokemonToEntry(const UPokemon& _Pokemon)
{
	if (PokemonEntry.size() == 6)
	{
		AddPokemonToComputer(_Pokemon);
		return;
	}

	OwnPokemon(_Pokemon.GetPokedexNo());
	PokemonEntry.push_back(_Pokemon);
}

void UUserData::AddPokemonToComputer(const UPokemon& _Pokemon)
{
	PokemonInComputer.push_back(_Pokemon);
}

void UUserData::SwapEntry(int _Index0, int _Index1)
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

int UUserData::GetPokemonEntrySize()
{
	return static_cast<int>(PokemonEntry.size());
}

bool UUserData::IsAchieved(EAchievement _Achievement)
{
	if (false == AchievementMap.contains(_Achievement))
	{
		AchievementMap[_Achievement] = false;
		return false;
	}

	return AchievementMap[_Achievement];
}

void UUserData::Achieve(EAchievement _Achievement)
{
	AchievementMap[_Achievement] = true;
}

void UUserData::Unachieve(EAchievement _Achievement)
{
	AchievementMap[_Achievement] = false;
}

void UUserData::SeePokemon(EPokemonId _PokedexNo)
{
	PokedexSeenMap[_PokedexNo] = true;
}

void UUserData::OwnPokemon(EPokemonId _PokedexNo)
{
	PokedexSeenMap[_PokedexNo] = true;
	PokedexOwnedMap[_PokedexNo] = true;
}

int UUserData::GetSeenPokemonCount()
{
	return static_cast<int>(PokedexSeenMap.size());
}

int UUserData::GetOwnedPokemonCount()
{
	return static_cast<int>(PokedexOwnedMap.size());
}

int UUserData::GetIdNo()
{
	if (IdNo == -1)
	{
		// 아직 IdNo 값이 할당되지 않은 경우 랜덤하게 값을 할당해서 반환한다.
		IdNo = UPokemonMath::RandomInt(0, 65535);
	}

	return IdNo;
}

std::wstring UUserData::GetIdNoW()
{
	return UPokemonString::PadLeft(std::to_wstring(GetIdNo()), 5, L'0');
}

std::wstring UUserData::GetNickNameW()
{
	return UEngineString::AnsiToUniCode(Nickname);
}

int UUserData::GetMoney()
{
	return Money;
}

void UUserData::LoseMoney(int _Money)
{
	if (_Money <= 0)
	{
		MsgBoxAssert("[UUserData::LoseMoney] 0 이하의 돈을 잃을 수 없습니다.");
		return;
	}

	Money -= _Money;

	if (Money < 0)
	{
		Money = 0;
	}
}

void UUserData::GainMoney(int _Money)
{
	if (_Money <= 0)
	{
		MsgBoxAssert("[UUserData::GainMoney] 0 이하의 돈을 얻을 수 없습니다.");
		return;
	}

	Money += _Money;
}

void UUserData::GainItem(EItemId _Id, int _Count)
{
	const FItem* Item = UGameData::FindItem(_Id);
	EItemType ItemType = Item->Type;

	if (false == Inventory.contains(ItemType) || false == Inventory[ItemType].contains(_Id))
	{
		Inventory[ItemType][_Id] = 0;
	}

	Inventory[ItemType][_Id] += _Count;
}

void UUserData::LoseItem(EItemId _Id, int _Count)
{
	const FItem* Item = UGameData::FindItem(_Id);
	EItemType ItemType = Item->Type;

	if (false == Inventory.contains(ItemType) || false == Inventory[ItemType].contains(_Id))
	{
		MsgBoxAssert("[UUserData::LoseItem] 갖고 있지 않는 아이템을 잃을 수 없습니다.");
		return;
	}

	int HaveCount = Inventory[ItemType][_Id];

	if (_Count > HaveCount)
	{
		MsgBoxAssert("[UUserData::LoseItem] Id: " + std::to_string(static_cast<int>(_Id)) + " 아이템을 " 
			+ std::to_string(HaveCount) + "개만 갖고 있으므로 " + std::to_string(_Count) + "개를 잃을 수 없습니다.");
		return;
	}

	Inventory[ItemType][_Id] -= _Count;

	if (Inventory[ItemType][_Id] <= 0)
	{
		Inventory[ItemType].erase(_Id);
	}
}

std::list<FInventoryRecord> UUserData::GetItemList(EItemType _ItemType, int _StartIndex, int _EndIndex)
{
	const std::map<EItemId, int>& Map = Inventory[_ItemType];

	if (_StartIndex > _EndIndex)
	{
		MsgBoxAssert("[UUserData::GetItemList] 인덱스를 잘못 설정했습니다. 시작 인덱스가 종료 인덱스보다 큽니다.");
		return {};
	}

	if (_StartIndex < 0)
	{
		MsgBoxAssert("[UUserData::GetItemList] 인덱스를 잘못 설정했습니다. 시작 인덱스가 0보다 작습니다.");
		return {};
	}

	if (_EndIndex >= Map.size())
	{
		MsgBoxAssert("[UUserData::GetItemList] 인덱스를 잘못 설정했습니다. 종료 인덱스가 맵의 크기 이상입니다.");
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

const FItem* UUserData::GetItem(EItemType _ItemType, int _Index)
{
	const std::map<EItemId, int>& Map = Inventory[_ItemType];

	if (_Index >= Map.size())
	{
		MsgBoxAssert("[UUserData::GetItem] 인덱스를 잘못 설정했습니다. 인덱스가 맵의 크기 이상입니다.");
		return {};
	}

	std::map<EItemId, int>::const_iterator StartIter = Map.begin();
	StartIter = std::next(StartIter, _Index);

	EItemId Id = StartIter->first;

	return UGameData::FindItem(Id);
}

int UUserData::GetRecordCount(EItemType _ItemType)
{
	return static_cast<int>(Inventory[_ItemType].size());
}

