#pragma once
#include <vector>
#include <string>
#include <EngineBase/EngineDebug.h>

enum class EPokedexNo
{
	None,
	Bulbasaur = 1,
	Charmander = 4,
	Squirtle = 7,
};

enum class EAbility
{
	NONE,
	Overgrow,
	Blaze,
	Torrent,
	ShieldDust,
	ShedSkin,
	CompoundEyes,
	Swarm,
	KeenEye,
	RunAway,
	Guts,
	Intimidate,
	Static,
	SandVeil,
	PoisonPoint,
};

enum class EGender
{
	Male,
	Female
};

enum class ENature
{
	NONE,		// Inc (x1.1)	Dec (x0.9)
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
	Careful,		// SpDef		SpAtk
	MAX,
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
	NONE,		// 레벨 100 기준 누적 경험치		누적 경험치 공식
	Erratic,	// 600,000						piecewise
	Fast,		// 800,000						0.8n^3
	MediumFast,	// 1,000,000					n^3
	MediumSlow,	// 1,059,860					1.2n^3 - 15n^2 + 100n - 140
	Slow,		// 1,250,000					1.25n^3
	Fluctuating	// 1,640,000					piecewise
};

enum class EPokemonType
{
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
	UPokemon(const UPokemon& _Other) = default;
	UPokemon(UPokemon&& _Other) noexcept = default;
	UPokemon& operator=(const UPokemon& _Other) = default;
	UPokemon& operator=(UPokemon&& _Other) noexcept = default;

	std::wstring GetName()
	{
		return Name;
	}

	int GetLevel() const
	{
		return Level;
	}

	int GetCurHp() const
	{
		return CurHp;
	}

	void Heal(int _Value)
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

	void HealAll()
	{
		CurHp = GetHp();
	}

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

	void SetLevel(int _Level)
	{
		Level = _Level;
	}

protected:
	// Base Stat
	int BHp = 0;
	int BAtk = 0;
	int BDef = 0;
	int BSpAtk = 0;
	int BSpDef = 0;
	int BSpeed = 0;

	// Effort Value Yield (기절했을 때 상대에게 주는 노력치)
	int YHp = 0;
	int YAtk = 0;
	int YDef = 0;
	int YSpAtk = 0;
	int YSpDef = 0;
	int YSpeed = 0;

	// 기본 정보
	std::wstring Name;
	EPokedexNo PokedexNo = EPokedexNo::None;
	int Level = 0;
	int AccExp = 0;
	int CurHp = 0;

	// Nature
	ENature Nature = ENature::NONE;

	// 기타
	EAbility Ability = EAbility::NONE;
	EExperienceGroup ExpGroup = EExperienceGroup::NONE;
	std::vector<EPokemonType> Types;
	int CatchRate = 0;
	int Friendship = 0;

	// 함수
	void SetRandomIVs();
	void SetRandomGender(float _MaleRatio);
	void SetRandomNature();
	void SetRandomAbility(std::vector<EAbility> _Abilities);
	int GetAccExpForLevel(int _Level) const;
	int GetErraticAccExpForLevel(int _Level) const;
	int GetFluctuatingAccExpForLevel(int _Level) const;

private:
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

	// 기타
	EGender Gender = EGender::Male;
};

