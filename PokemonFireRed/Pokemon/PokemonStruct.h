#pragma once
#include <map>
#include <string>
#include <vector>
#include <EngineBase/EngineString.h>

enum class EPokedexNo
{
	None,
	Bulbasaur = 1,
	Ivysaur = 2,
	Venusaur = 3,
	Charmander = 4,
	Charmeleon = 5,
	Charizard = 6,
	Squirtle = 7,
	Wartortle = 8,
	Blastoise = 9,
	Caterpie = 10,
	Metapod = 11,
	Butterfree = 12,
	Weedle = 13,
	Kakuna = 14,
	Beedrill = 15,
	Pidgey = 16,
	Pidgeotto = 17,
	Pidgeot = 18,
	Rattata = 19,
	Raticate = 20,
	Spearow = 21,
	Fearow = 22,
	Ekans = 23,
	Arbok = 24,
	Pikachu = 25,
	Raichu = 26,
	Sandshrew = 27,
	Sandslash = 28,
	NidoranFemale = 29,
	Nidorina = 30,
	Nidoqueen = 31,
	NidoranMale = 32,
	Nidorino = 33,
	Nidoking = 34,
	Mankey = 56,
	Diglett = 50,
	Geodude = 74,
	Onix = 95,
};

enum class EPokemonAbility
{
	None,
	Overgrow,
	Blaze,
	Torrent,
	ShieldDust,
	ShedSkin,
	CompoundEyes,
	Swarm,
	KeenEye,
	RunAway,
	Guts = 10,
	Intimidate,
	Static,
	SandVeil,
	PoisonPoint,
	VitalSpirit,
	RockHead
};

struct FPokemonAbility
{
public:
	EPokemonAbility Id = EPokemonAbility::None;
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
	EPokemonGender Id = EPokemonGender::Male;
	std::string ImageName;
	std::string BigImageName;
};

enum class EPokemonNature
{
	None,		// Inc (x1.1)	Dec (x0.9)
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
	EPokemonNature Id = EPokemonNature::None;
	std::string Name = "None";
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
	None,
	Sleep,
	Poison,
	Burn,
	Freeze,
	Paralysis,
};

struct FPokemonStatus
{
public:
	EPokemonStatus Id = EPokemonStatus::None;
	std::string Name;
	std::string ImageName;
};

enum class EPokemonType
{
	None,
	Normal,
	Fire,
	Water,
	Electric,
	Grass,
	Ice,
	Fighting,
	Poison,
	Ground,
	Flying,
	Psychic,
	Bug,
	Rock,
	Ghost,
	Dragon,
	Dark,
	Steel
};

struct FPokemonType
{
public:
	EPokemonType Id = EPokemonType::Normal;
	std::string Name;
	std::string ImageName;

	std::wstring GetNameW() const
	{
		return UEngineString::AnsiToUniCode(Name);
	}

	// <���ǻ���>
	// map::operator[]�� key�� ������ ���ο� ���Ҹ� insert�ϴ� ������� �����Ѵ�.
	// ��, ������ �߱��ϱ� ������ const FPokemonType������ ����� �� ���� �����̴�.
	// ���Ҹ� ��ȸ�ϰ� �ʹٸ� map::at �Լ��� ��ȸ�ؾ� �Ѵ�.

	// EffectTo[EnemyType] = ���� ������ �� Ÿ������ ���� ������ ����
	std::map<EPokemonType, float> EffectTo;
};

enum class EPokemonMove
{
	None,
	Absorb,
	Bubble,
	Ember,
	Growl,
	Gust,
	LeechSeed,
	QuickAttack,
	SandAttack,
	Scratch,
	Tackle,
	TailWhip,
	StringShot,
	PoisonString,
	Peck,
	Leer,
	LowKick,
	DefenseCurl,
	Bind,
	RockThrow
};

enum class ESecondaryEffectTarget
{
	None,
	Self,
	Enemy,
};

enum class ESecondaryEffectStatStage
{
	None,
	Atk,
	Def,
	SpAtk,
	SpDef,
	Speed,
	Accuracy,
	Evasion
};

struct FPokemonMove
{
public:
	EPokemonMove Id = EPokemonMove::None;
	EPokemonType TypeId = EPokemonType::Normal;
	std::string Name;
	std::string Explain;
	int PP = 0;
	int BasePower = 0;
	int Accuracy = 100;
	bool IsContact = false;
	
	// �ΰ� ȿ��
	ESecondaryEffectTarget SETarget = ESecondaryEffectTarget::None;
	int SERate = 0;
	ESecondaryEffectStatStage SEStatStageId = ESecondaryEffectStatStage::None;
	int SEStatStageValue = 0;
	EPokemonStatus SEStatusId = EPokemonStatus::None;

	std::wstring GetNameW() const
	{
		return UEngineString::AnsiToUniCode(Name);
	}

	std::wstring GetExplainW() const
	{
		return UEngineString::AnsiToUniCode(Explain);
	}

	bool IsPhysical() const
	{
		return TypeId == EPokemonType::Normal
			|| TypeId == EPokemonType::Fighting
			|| TypeId == EPokemonType::Flying
			|| TypeId == EPokemonType::Poison
			|| TypeId == EPokemonType::Ground
			|| TypeId == EPokemonType::Rock
			|| TypeId == EPokemonType::Bug
			|| TypeId == EPokemonType::Ghost
			|| TypeId == EPokemonType::Steel;
	}
};

enum class EExperienceGroup
{
	None,		// ���� 100 ���� ���� ����ġ		���� ����ġ ����
	Erratic,	// 600,000						piecewise
	Fast,		// 800,000						0.8n^3
	MediumFast,	// 1,000,000					n^3
	MediumSlow,	// 1,059,860					1.2n^3 - 15n^2 + 100n - 140
	Slow,		// 1,250,000					1.25n^3
	Fluctuating	// 1,640,000					piecewise
};

// ������ �ɷ�ġ�� �����ϴ� ����ü
struct FPokemonSpecies
{
public:
	void AddLevelUpMove(int _Level, EPokemonMove _Move)
	{
		LevelUpMoves[_Level].push_back(_Move);
	}

	// Base Stat
	int BHp = 0;
	int BAtk = 0;
	int BDef = 0;
	int BSpAtk = 0;
	int BSpDef = 0;
	int BSpeed = 0;

	// Effort Value Yield (�������� �� ��뿡�� �ִ� ���ġ)
	int YHp = 0;
	int YAtk = 0;
	int YDef = 0;
	int YSpAtk = 0;
	int YSpDef = 0;
	int YSpeed = 0;

	// ��Ÿ
	EPokedexNo Id = EPokedexNo::None;
	std::string Name;
	EExperienceGroup ExpGroup = EExperienceGroup::None;
	std::vector<EPokemonType> TypeIds;
	std::vector<EPokemonAbility> AbilityCandidateIds;
	std::map<int, std::vector<EPokemonMove>> LevelUpMoves;
	float MaleRatio = 0.0f;
	int CatchRate = 0;
	int Friendship = 0;
};