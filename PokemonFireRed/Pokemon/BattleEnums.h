#pragma once

enum class EEnemyType
{
	Wild,
	Trainer,
	GymLeader
};

enum class EBattleAction
{
	None,
	Fight,
	Escape,
	Shift,
	UseItem,
	Ball,
};

enum class ETypeVs
{
	HasNoEffect,
	NotVeryEffective,
	NormallyEffective,
	SuperEffective,
};

struct FDamageResult
{
public:
	int Damage = 0;
	bool IsCritical = false;
	ETypeVs TypeVs = ETypeVs::NormallyEffective;

	std::wstring GetTypeVsW(std::wstring _DefenderName) const
	{
		switch (TypeVs)
		{
		case ETypeVs::HasNoEffect:
			return  L"It doesn't affect\nWild " + _DefenderName + L"¡¦";
		case ETypeVs::NotVeryEffective:
			return L"It's not very effective¡¦";
		case ETypeVs::SuperEffective:
			return L"It's super effective!";
		default:
			break;
		}
		return L"";
	}
};