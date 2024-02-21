#pragma once
#include <vector>

enum class EAbility
{
	None,
	Overgrow,

};

enum class EGender
{
	Male,
	Female
};

enum class ENature
{
	None,		// Inc (x1.1)	Dec (x0.9)
	Hardy,		// None			None
	Lonely,		// Atk			Def
	Brave,		// Atk			Speed
	Adamant,	// Atk			SpAtk
	Naughty,	// Atk			SpDef
	Docile,		// None			None
	Bold,		// Def			Atk
	Relaxed,	// Def			Speed
	Impish,		// Def			SpAtk
	Lax,		// Def			SpDef
	Serious,	// None			None
	Timid,		// Speed		Atk
	Hasty,		// Speed		Def
	Jolly,		// Speed		SpAtk
	Naive,		// Speed		SpDef
	Bashful,	// None			None
	Modest,		// SpAtk		Atk
	Mild,		// SpAtk		Def
	Quiet,		// SpAtk		Speed
	Rash,		// SpAtk		SpDef
	Quirky,		// None			None
	Calm,		// SpDef		Atk
	Gentle,		// SpDef		Def
	Sassy,		// SpDef		Speed
	Careful		// SpDef		SpAtk
};

enum class EPokemonStatus
{
	Normal,
	Sleep,
	Poison,
	Burn,
	Freeze,
	Paralysis,
};

// 종에 소속
enum class EExperienceGroup
{	
	None,		// 레벨 100 기준 누적 경험치		누적 경험치 공식
	Erratic,	// 600,000						piecewise
	Fast,		// 800,000						0.8n^3
	MediumFast,	// 1,000,000					n^3
	MediumSlow,	// 1,059,860					1.2n^3 - 15n^2 + 100n - 140
	Slow,		// 1,250,000					1.25n^3
	Fluctuating	// 1,640,000					piecewise
};

enum class EPokemonType
{
	None,
	Normal,
	Fighting,
	Flying,
	Poison,
	Ground,
	Rock,
	Bug,
	Ghost,
	Steel,
	Fire,
	Water,
	Grass,
	Electric,
	Psychic,
	Ice,
	Dragon,
	Dark,
	Fairy
};

class UPokemon
{
public:
	// constructor destructor
	UPokemon();
	~UPokemon();

	// delete Function
	UPokemon(const UPokemon& _Other) = delete;
	UPokemon(UPokemon&& _Other) noexcept = delete;
	UPokemon& operator=(const UPokemon& _Other) = delete;
	UPokemon& operator=(UPokemon&& _Other) noexcept = delete;

	int GetHp() const;
	int GetAtk() const;
	int GetDef() const;
	int GetSpAtk() const;
	int GetSpDef() const;
	int GetSpeed() const;

	int GetAccExp() const
	{
		return AccExp;
	}
	
	int GetExpForNextLevel() const;

	// 디버그
	void SetLevel(int _Level)
	{
		Level = _Level;
	}

	void SetExpGroup(EExperienceGroup _ExpGroup)
	{
		ExpGroup = _ExpGroup;
	}

protected:

private:
	// 레벨과 누적 경험치
	int Level = 0;
	int AccExp = 0;

	// 현재 체력
	int CurHp = 0;

	// Base Stat
	int BHp = 0;
	int BAtk = 0;
	int BDef = 0;
	int BSpAtk = 0;
	int BSpDef = 0;
	int BSpeed = 0;

	// Individual Value
	int IHp = 0;
	int IAtk = 0;
	int IDef = 0;
	int ISpAtk = 0;
	int ISpDef = 0;
	int ISpeed = 0;

	// Effort Value
	int EHp = 0;
	int EAtk = 0;
	int EDef = 0;
	int ESpAtk = 0;
	int ESpDef = 0;
	int ESpeed = 0;

	// Nature
	ENature Nature = ENature::None;

	// 기타
	EGender Gender = EGender::Male;
	EAbility Ability = EAbility::None;
	EExperienceGroup ExpGroup = EExperienceGroup::None;
	std::vector<EPokemonType> Types;

	int GetAccExpForLevel(int _Level) const;
	int GetErraticAccExpForLevel(int _Level) const;
	int GetFluctuatingAccExpForLevel(int _Level) const;
};

