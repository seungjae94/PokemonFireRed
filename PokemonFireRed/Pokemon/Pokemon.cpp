#include "Pokemon.h"
#include "PokemonMath.h"
#include <random>

UPokemon::UPokemon(EPokedexNo _Id, int _Level)
	: Level(_Level)
{
	Species = UPokemonDB::FindSpecies(_Id);
	Name = UPokemonString::ToUpperW(Species->SpeciesName);
	Level = 1;
	AccExp = 0;
	InitRandomIVs();
	InitRandomGender();
	InitRandomNature();
	InitRandomAbility();
	CurHp = GetHp();
}

UPokemon::~UPokemon()
{
}

std::vector<std::string> UPokemon::GetTypeImageNames(bool _PlaceHolder) const
{
	std::vector<std::string> Result;
	int TypeSize = static_cast<int>(Species->TypeIds.size());

	for (int i = 0; i < TypeSize; ++i)
	{
		EPokemonType TypeId = Species->TypeIds[i];
		Result.push_back(UPokemonDB::FindType(TypeId)->ImageName);
	}
	for (int i = TypeSize; i < 2; ++i)
	{
		Result.push_back(RN::TypePlaceHolder);
	}

	return Result;
}

void UPokemon::Heal(int _Value)
{
	if (_Value < 0)
	{
		MsgBoxAssert("음수만큼 HP를 회복할 수 없습니다.");
		return;
	}

	CurHp += _Value;
	if (CurHp > GetHp())
	{
		CurHp = GetHp();
	}
}

int UPokemon::GetHp() const
{
	return (2 * Species->BHp + IHp + EHp / 4) * Level / 100 + Level + 10;
}

int UPokemon::GetAtk() const
{
	return UPokemonMath::Floor(((2 * Species->BAtk + IAtk + EAtk / 4) * Level / 100 + 5) * Nature->NAtk);
}

int UPokemon::GetDef() const
{
	return UPokemonMath::Floor(((2 * Species->BDef + IDef + EDef / 4) * Level / 100 + 5) * Nature->NDef);
}

int UPokemon::GetSpAtk() const
{
	return UPokemonMath::Floor(((2 * Species->BSpAtk + ISpAtk + ESpAtk / 4) * Level / 100 + 5) * Nature->NSpAtk);
}

int UPokemon::GetSpDef() const
{
	return UPokemonMath::Floor(((2 * Species->BSpDef + ISpDef + ESpDef / 4) * Level / 100 + 5) * Nature->NSpDef);
}

int UPokemon::GetSpeed() const
{
	return UPokemonMath::Floor(((2 * Species->BSpeed + ISpeed + ESpeed / 4) * Level / 100 + 5) * Nature->NSpeed);
}

void UPokemon::GainExp(int _Exp)
{
	if (Level >= 100)
	{
		return;
	}

	AccExp += _Exp;

	if (GetNextLevelExp() <= 0)
	{
		int PrevHp = GetHp();
		++Level;
		int NextHp = GetHp();
		CurHp += NextHp - PrevHp;
	}
}

int UPokemon::GetExpSize() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return GetAccExpForLevel(Level + 1) - GetAccExpForLevel(Level);
}

int UPokemon::GetNextLevelExp() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return GetAccExpForLevel(Level + 1) - AccExp;
}

int UPokemon::GetExp() const
{
	if (Level >= 100 || Level <= 0)
	{
		return 0;
	}

	return GetExpSize() - GetNextLevelExp();
}

void UPokemon::InitRandomIVs()
{
	IHp = UPokemonMath::RandomInt(0, 31);
	IAtk = UPokemonMath::RandomInt(0, 31);
	IDef = UPokemonMath::RandomInt(0, 31);
	ISpAtk = UPokemonMath::RandomInt(0, 31);
	ISpDef = UPokemonMath::RandomInt(0, 31);
	ISpeed = UPokemonMath::RandomInt(0, 31);
}

void UPokemon::InitRandomGender()
{
	float GenderInt = UPokemonMath::Random(0.0f, 1.0f);
	float MaleRatio = Species->MaleRatio;

	if (GenderInt <= MaleRatio)
	{
		Gender = UPokemonDB::FindGender(EPokemonGender::Male);
	}
	else
	{
		Gender = UPokemonDB::FindGender(EPokemonGender::Female);
	}
}

void UPokemon::InitRandomNature()
{
	int NatureInt = UPokemonMath::RandomInt(1, static_cast<int>(EPokemonNature::MAX) - 1);
	EPokemonNature NatureId = static_cast<EPokemonNature>(NatureInt);
	Nature = UPokemonDB::FindNature(NatureId);
}

void UPokemon::InitRandomAbility()
{
	int AbilitySize = static_cast<int>(Species->AbilityCandidateIds.size());
	int AbilityInt = UPokemonMath::RandomInt(0, AbilitySize - 1);
	EPokemonAbility AbilityId = Species->AbilityCandidateIds[AbilityInt];
	Ability = UPokemonDB::FindAbility(AbilityId);
}

int UPokemon::GetAccExpForLevel(int _Level) const
{
	if (_Level == 1)
	{
		return 0;
	}

	double Result = 0;

	switch (Species->ExpGroup)
	{
	case EExperienceGroup::Erratic:
		return GetErraticAccExpForLevel(_Level);
	case EExperienceGroup::Fast:
		Result = 0.8 * std::pow(_Level, 3);
		break;
	case EExperienceGroup::MediumFast:
		Result = std::pow(_Level, 3);
		break;
	case EExperienceGroup::MediumSlow:
		Result = 1.2 * std::pow(_Level, 3) - 15.0 * std::pow(_Level, 2) + 100 * _Level - 140;
		break;
	case EExperienceGroup::Slow:
		Result = 1.25 * std::pow(_Level, 3);
		break;
	case EExperienceGroup::Fluctuating:
		return GetFluctuatingAccExpForLevel(_Level);
	default:
		break;
	}

	return std::lround(std::floor(Result));
}

int UPokemon::GetErraticAccExpForLevel(int _Level) const
{
	if (_Level < 50)
	{
		return UPokemonMath::Pow(_Level, 3) * (100 - _Level) / 50;
	}
	else if (_Level < 68)
	{
		return UPokemonMath::Pow(_Level, 3) * (150 - _Level) / 100;
	}
	else if (_Level < 98)
	{
		return UPokemonMath::Pow(_Level, 3) * ((1911 - 10 * _Level) / 3) / 500;
	}

	return UPokemonMath::Pow(_Level, 3) * (160 - _Level) / 100;
}

int UPokemon::GetFluctuatingAccExpForLevel(int _Level) const
{
	if (_Level < 15)
	{
		return UPokemonMath::Pow(_Level, 3) * ((_Level + 1) / 3 + 24) / 50;
	}
	else if (_Level < 36)
	{
		return UPokemonMath::Pow(_Level, 3) * (_Level + 14) / 50;
	}

	return UPokemonMath::Pow(_Level, 3) * ((_Level / 2) + 32) / 50;
}
