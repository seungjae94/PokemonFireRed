#pragma once
#include <string>

enum class EItemId
{
	None,
	Potion,
	SuperPotion,
	HyperPotion,
	Antidote,
	BurnHeal,
	FullHeal,
	PokeBall = 200,
};

enum class EItemType
{
	None,
	UseItem,
	KeyItem,
	PokeBall
};

enum class EUseEffect
{
	None,
	Hp,
	CureAll,
	CureBurn,
	CurePoison,
};

struct FItem
{
public:
	EItemId Id = EItemId::None;
	EItemType Type = EItemType::UseItem;
	
	std::wstring Name;
	std::wstring Explain;
	int BuyPrice = 0;
	std::string ImageName;

	// 사용 효과
	EUseEffect UseEffect = EUseEffect::None;
	int HealValue = 0;
};

