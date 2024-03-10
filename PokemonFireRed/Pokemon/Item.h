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
};

enum class EItemType
{
	UseItem,
	KeyItem,
	PokeBall
};

enum class EHealType
{
	None,
	Hp,
	CureAll,
	CureBurn,
	CurePoison,
};

struct FUseItemEffect
{
public:
	EHealType HealType = EHealType::None;
	int HealValue = 0;
};

struct FItem
{
public:
	EItemId Id = EItemId::None;
	EItemType Type = EItemType::UseItem;
	FUseItemEffect UseItemEffect;
	std::wstring Name;
	std::wstring Explain;
	std::string ImageName;
};

