#pragma once
#include <string>
#include <vector>
#include <EngineBase/EngineString.h>

enum class EPokedexNo
{
	None,
	Bulbasaur = 1,
	Charmander = 4,
	Squirtle = 7,
};

enum class EPokemonAbility
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

struct FPokemonAbility
{
public:
	FPokemonAbility() 
	{}

	FPokemonAbility(
		EPokemonAbility _Id, 
		std::string_view _Name, 
		std::string_view _Explain
	) : Id(_Id), Name(_Name), Explain(_Explain)
	{}

	EPokemonAbility Id;
	std::string Name;
	std::string Explain;

	std::wstring GetNameW() const
	{
		return UEngineString::AnsiToUniCode(Name);
	}

	std::wstring GetExplainW() const
	{
		return UEngineString::AnsiToUniCode(Explain);
	}
};

enum class EPokemonGender
{
	Male,
	Female
};

struct FPokemonGender
{
public:
	FPokemonGender()
	{}

	FPokemonGender(
		EPokemonGender _Id, 
		std::string_view _ImageName,
		std::string_view _BigImageName
	)
		: Id(_Id), ImageName(_ImageName), BigImageName(_BigImageName)
	{}

	EPokemonGender Id;
	std::string ImageName;
	std::string BigImageName;
};

enum class EPokemonNature
{
	NONE,		// Inc (x1.1)	Dec (x0.9)
	Adamant,	// Atk			SpAtk
	Bashful,	// None			None
	Bold,		// Def			Atk
	Brave,		// Atk			Speed
	Calm,		// SpDef		Atk
	Careful,	// SpDef		SpAtk
	Docile,		// None			None
	Gentle,		// SpDef		Def
	Hardy,		// None			None
	Hasty,		// Speed		Def
	Impish,		// Def			SpAtk
	Jolly,		// Speed		SpAtk
	Lax,		// Def			SpDef
	Lonely,		// Atk			Def
	Mild,		// SpAtk		Def
	Modest,		// SpAtk		Atk
	Naive,		// Speed		SpDef
	Naughty,	// Atk			SpDef
	Quiet,		// SpAtk		Speed
	Quirky,		// None			None
	Rash,		// SpAtk		SpDef
	Relaxed,	// Def			Speed
	Sassy,		// SpDef		Speed
	Serious,	// None			None
	Timid,		// Speed		Atk
	MAX,
};

struct FPokemonNature
{
public:
	FPokemonNature()
	{}

	FPokemonNature(
		EPokemonNature _Id, std::string_view _Name,
		float _NAtk, float _NDef, float _NSpAtk, float _NSpDef, float _NSpeed
	) : Id(_Id), Name(_Name), NAtk(_NAtk), NDef(_NDef), NSpAtk(_NSpAtk), NSpDef(_NSpDef), NSpeed(_NSpeed)
	{}
	EPokemonNature Id;
	std::string Name;
	float NAtk = 1.0f;
	float NDef = 1.0f;
	float NSpAtk = 1.0f;
	float NSpDef = 1.0f;
	float NSpeed = 1.0f;

	std::wstring GetNameW() const
	{
		return UEngineString::AnsiToUniCode(Name);
	}
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

struct FPokemonStatus
{
public:
	FPokemonStatus()
	{}

	FPokemonStatus(
		EPokemonStatus _Id, std::string_view _ImageName
	)
		: Id(_Id), ImageName(_ImageName)
	{}
	EPokemonStatus Id;
	std::string ImageName;
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

struct FPokemonType
{
public:
	FPokemonType()
	{}

	FPokemonType(
		EPokemonType _Id, std::string_view _ImageName
	)
		: Id(_Id), ImageName(_ImageName)
	{}

	EPokemonType Id;
	std::string ImageName;
};

enum class EPokemonMove
{
	None,
	Tackle,
	Growl,
	Absorb,
	LeechSeed,
	TailWhip,
	Scratch,
	Ember,
	Bubble,
	SandAttack,
	Gust,
	QuickAttack
};

struct FPokemonMove
{
public:
	FPokemonMove()
	{}

	FPokemonMove(
		EPokemonMove _Id,
		std::string_view _Name, std::string_view _Explain,
		EPokemonType _Type,
		int _PP, int _BasePower, int _Accuracy
	)
		: Id(_Id), Name(_Name), Explain(_Explain), Type(_Type), 
		PP(_PP), BasePower(_BasePower), Accuracy(_Accuracy)
	{}

	EPokemonMove Id;
	std::string Name;
	std::string Explain;
	EPokemonType Type;
	int PP = 0;
	int BasePower = 0;
	int Accuracy = 100;

	std::wstring GetNameW()
	{
		UEngineString::AnsiToUniCode(Name);
	}

	std::wstring GetExplainW()
	{
		UEngineString::AnsiToUniCode(Explain);
	}
};

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

// 종족별 능력치를 저장하는 구조체
struct FPokemonSpecies
{
public:
	FPokemonSpecies()
	{}

	FPokemonSpecies(
		EPokedexNo _PokedexNo, std::string_view _SpeciesName,
		int _BHp, int _BAtk, int _BDef, int _BSpAtk, int _BSpDef, int _BSpeed,
		int _YHp, int _YAtk, int _YDef, int _YSpAtk, int _YSpDef, int _YSpeed,
		EExperienceGroup _ExpGroup,
		const std::vector<EPokemonType>& _Types,
		const std::vector<EPokemonAbility>& _AbilityCandidates,
		float _MaleRatio, int _CatchRate, int _Friendship
	)
		: PokedexNo(_PokedexNo), SpeciesName(_SpeciesName), 
		BHp(_BHp), BAtk(_BAtk), BDef(_BDef), BSpAtk(_BSpAtk), BSpDef(_BSpDef), BSpeed(_BSpeed),
		YHp(_YHp), YAtk(_YAtk), YDef(_YDef), YSpAtk(_YSpAtk), YSpDef(_YSpDef), YSpeed(_YSpeed),
		ExpGroup(_ExpGroup), Types(_Types), AbilityCandidates(_AbilityCandidates),
		MaleRatio(_MaleRatio), CatchRate(_CatchRate), Friendship(_Friendship)
	{}

	void AddLevelUpMove(int _Level, EPokemonMove _Move)
	{
		LevelUpMoves[_Level] = _Move;
	}

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

	// 기타
	std::string SpeciesName;
	EPokedexNo PokedexNo = EPokedexNo::None;
	EExperienceGroup ExpGroup = EExperienceGroup::NONE;
	std::vector<EPokemonType> Types;
	std::vector<EPokemonAbility> AbilityCandidates;
	std::map<int, EPokemonMove> LevelUpMoves;
	float MaleRatio = 0.0f;
	int CatchRate = 0;
	int Friendship = 0;
};

