#include "Pokemon.h"
#include "PokemonMath.h"
#include <random>

UPokemon::UPokemon()
{
}

UPokemon::~UPokemon()
{
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
	return (2 * BHp + IHp + EHp / 4) * Level / 100 + Level + 10;
}

int UPokemon::GetAtk() const
{
	float N = 1.0f;
	switch (Nature)
	{
	case ENature::Lonely:
	case ENature::Brave:
	case ENature::Adamant:
	case ENature::Naughty:
		N = 1.1f;
		break;
	case ENature::Bold:
	case ENature::Timid:
	case ENature::Modest:
	case ENature::Calm:
		N = 0.9f;
		break;
	default:
		break;
	}

	return UPokemonMath::Floor(((2 * BAtk + IAtk + EAtk / 4) * Level / 100 + 5) * N);
}

int UPokemon::GetDef() const
{
	float N = 1.0f;
	switch (Nature)
	{
	case ENature::Bold:
	case ENature::Relaxed:
	case ENature::Impish:
	case ENature::Lax:
		N = 1.1f;
		break;
	case ENature::Lonely:
	case ENature::Hasty:
	case ENature::Mild:
	case ENature::Gentle:
		N = 0.9f;
		break;
	default:
		break;
	}

	return UPokemonMath::Floor(((2 * BDef + IDef + EDef / 4) * Level / 100 + 5) * N);
}

int UPokemon::GetSpAtk() const
{
	float N = 1.0f;
	switch (Nature)
	{
	case ENature::Modest:
	case ENature::Mild:
	case ENature::Quiet:
	case ENature::Rash:
		N = 1.1f;
		break;
	case ENature::Adamant:
	case ENature::Impish:
	case ENature::Jolly:
	case ENature::Careful:
		N = 0.9f;
		break;
	default:
		break;
	}

	return UPokemonMath::Floor(((2 * BSpAtk + ISpAtk + ESpAtk / 4) * Level / 100 + 5) * N);
}

int UPokemon::GetSpDef() const
{
	float N = 1.0f;
	switch (Nature)
	{
	case ENature::Calm:
	case ENature::Gentle:
	case ENature::Sassy:
	case ENature::Careful:
		N = 1.1f;
		break;
	case ENature::Naughty:
	case ENature::Lax:
	case ENature::Naive:
	case ENature::Rash:
		N = 0.9f;
		break;
	default:
		break;
	}

	return UPokemonMath::Floor(((2 * BSpDef + ISpDef + ESpDef / 4) * Level / 100 + 5) * N);
}

int UPokemon::GetSpeed() const
{
	float N = 1.0f;
	switch (Nature)
	{
	case ENature::Timid:
	case ENature::Hasty:
	case ENature::Jolly:
	case ENature::Naive:
		N = 1.1f;
		break;
	case ENature::Brave:
	case ENature::Relaxed:
	case ENature::Quiet:
	case ENature::Sassy:
		N = 0.9f;
		break;
	default:
		break;
	}

	return UPokemonMath::Floor(((2 * BSpeed + ISpeed + ESpeed / 4) * Level / 100 + 5) * N);
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

void UPokemon::SetRandomIVs()
{
	IHp = UPokemonMath::RandomInt(0, 31);
	IAtk = UPokemonMath::RandomInt(0, 31);
	IDef = UPokemonMath::RandomInt(0, 31);
	ISpAtk = UPokemonMath::RandomInt(0, 31);
	ISpDef = UPokemonMath::RandomInt(0, 31);
	ISpeed = UPokemonMath::RandomInt(0, 31);
}

void UPokemon::SetRandomGender(float _MaleRatio)
{
	float GenderInt = UPokemonMath::Random(0.0f, 1.0f);

	if (GenderInt <= _MaleRatio)
	{
		Gender = EGender::Male;
	}
	else
	{
		Gender = EGender::Female;
	}
}

void UPokemon::SetRandomNature()
{
	int NatureInt = UPokemonMath::RandomInt(1, static_cast<int>(ENature::MAX) - 1);
	Nature = static_cast<ENature>(NatureInt);
}

void UPokemon::SetRandomAbility(std::vector<EAbility> _Abilities)
{
	int AbilitySize = static_cast<int>(_Abilities.size());
	int AbilityInt = UPokemonMath::RandomInt(0, AbilitySize - 1);
	Ability = _Abilities[AbilityInt];
}

int UPokemon::GetAccExpForLevel(int _Level) const
{
	if (_Level == 1)
	{
		return 0;
	}

	double Result = 0;

	switch (ExpGroup)
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
