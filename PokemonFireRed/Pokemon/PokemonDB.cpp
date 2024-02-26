#include "PokemonDB.h"

std::map<EPokedexNo, FPokemonSpecies> UPokemonDB::Species;
std::map<EPokemonMove, FPokemonMove> UPokemonDB::Moves;
std::map<EPokemonNature, FPokemonNature> UPokemonDB::Natures;
std::map<EPokemonAbility, FPokemonAbility> UPokemonDB::Abilities;
std::map<EPokemonGender, FPokemonGender> UPokemonDB::Genders;
std::map<EPokemonStatus, FPokemonStatus> UPokemonDB::Status;
std::map<EPokemonType, FPokemonType> UPokemonDB::Types;
std::list<EPokedexNo> UPokemonDB::ImplementedSpeciesNo;

class PokemonDBInitiator
{
public:
	PokemonDBInitiator()
	{
		FPokemonSpecies Bulbasaur = FPokemonSpecies(
			EPokedexNo::Bulbasaur, "BULBASAUR",
			45, 49, 49, 65, 65, 45,
			0, 0, 0, 1, 0, 0,
			EExperienceGroup::MediumSlow,
			{ EPokemonType::Grass, EPokemonType::Poison },
			{ EPokemonAbility::Overgrow },
			7.0f / 8, 45, 70
		);
		Bulbasaur.AddLevelUpMove(0, EPokemonMove::Tackle);
		Bulbasaur.AddLevelUpMove(4, EPokemonMove::Growl);
		Bulbasaur.AddLevelUpMove(7, EPokemonMove::LeechSeed);
		UPokemonDB::Species[EPokedexNo::Bulbasaur] = Bulbasaur;

		FPokemonSpecies Charmander = FPokemonSpecies(
			EPokedexNo::Bulbasaur, "CHARMANDER",
			39, 52, 43, 60, 50, 65,
			0, 0, 0, 0, 0, 1,
			EExperienceGroup::MediumSlow,
			{ EPokemonType::Fire },
			{ EPokemonAbility::Blaze },
			7.0f / 8, 45, 70
		);
		Charmander.AddLevelUpMove(0, EPokemonMove::Scratch);
		Charmander.AddLevelUpMove(0, EPokemonMove::Growl);
		Charmander.AddLevelUpMove(7, EPokemonMove::Ember);
		UPokemonDB::Species[EPokedexNo::Charmander] = Charmander;

		FPokemonSpecies Squirtle = FPokemonSpecies(
			EPokedexNo::Squirtle, "SQUIRTLE",
			44, 48, 65, 50, 64, 43,
			0, 0, 1, 0, 0, 0,
			EExperienceGroup::MediumSlow,
			{ EPokemonType::Water },
			{ EPokemonAbility::Torrent },
			7.0f / 8, 45, 70
		);
		Squirtle.AddLevelUpMove(0, EPokemonMove::Tackle);
		Squirtle.AddLevelUpMove(4, EPokemonMove::TailWhip);
		Squirtle.AddLevelUpMove(7, EPokemonMove::Bubble);
		UPokemonDB::Species[EPokedexNo::Squirtle] = Squirtle;

		UPokemonDB::ImplementedSpeciesNo.push_back(EPokedexNo::Bulbasaur);
		UPokemonDB::ImplementedSpeciesNo.push_back(EPokedexNo::Charmander);
		UPokemonDB::ImplementedSpeciesNo.push_back(EPokedexNo::Squirtle);

		UPokemonDB::Moves[EPokemonMove::Absorb] = FPokemonMove(
			EPokemonMove::Absorb,
			"ABSORB", "	An attack that absorbs\nhalf the damage it\ninflicted to restore HP.",
			EPokemonType::Grass,
			20, 20, 100
		);
		UPokemonDB::Moves[EPokemonMove::Bubble] = FPokemonMove(
			EPokemonMove::Bubble,
			"BUBBLE", "A spray of bubbles hits the foe. It may lower the foe¡¯s SPEED stat.",
			EPokemonType::Water,
			30, 20, 100
		);
		UPokemonDB::Moves[EPokemonMove::Ember] = FPokemonMove(
			EPokemonMove::Ember,
			"EMBER", "The foe is attacked with small flames. The foe may suffer a burn.",
			EPokemonType::Fire,
			25, 40, 100
		);
		UPokemonDB::Moves[EPokemonMove::Growl] = FPokemonMove(
			EPokemonMove::Growl,
			"GROWL", "The user growls in a cute way, making the foe lower its ATTACK stat.",
			EPokemonType::Normal,
			40, 0, 100
		);
		UPokemonDB::Moves[EPokemonMove::Gust] = FPokemonMove(
			EPokemonMove::Gust,
			"GUST", "Strikes the foe with a gust of wind whipped up by wings.",
			EPokemonType::Flying,
			35, 40, 100
		);
		UPokemonDB::Moves[EPokemonMove::LeechSeed] = FPokemonMove(
			EPokemonMove::LeechSeed,
			"LEECH SEED", "A seed is planted on the foe to steal some HP for the user on every turn.",
			EPokemonType::Grass,
			10, 0, 90
		);
		UPokemonDB::Moves[EPokemonMove::QuickAttack] = FPokemonMove(
			EPokemonMove::QuickAttack,
			"QUICK ATTACK", "An almost invisibly fast attack that is certain to strike first.",
			EPokemonType::Normal,
			30, 40, 100
		);
		UPokemonDB::Moves[EPokemonMove::SandAttack] = FPokemonMove(
			EPokemonMove::SandAttack,
			"SAND ATTACK", "A lot of sand is hurled in the foe¡¯s face, reducing its accuracy.",
			EPokemonType::Ground,
			15, 0, 100
		);
		UPokemonDB::Moves[EPokemonMove::Scratch] = FPokemonMove(
			EPokemonMove::Scratch,
			"SCRATCH", "Hard, pointed, and\nsharp claws rake\nthe foe.",
			EPokemonType::Normal,
			35, 40, 100
		);
		UPokemonDB::Moves[EPokemonMove::TailWhip] = FPokemonMove(
			EPokemonMove::TailWhip,
			"TAIL WHIP", "The user wags its tail cutely, making the foe lower its DEFENSE stat.",
			EPokemonType::Normal,
			30, 0, 100
		);
		UPokemonDB::Moves[EPokemonMove::Tackle] = FPokemonMove(
			EPokemonMove::Tackle,
			"TACKLE", "A physical attack in which the user charges, full body, into the foe.",
			EPokemonType::Normal,
			35, 35, 95
		);

		UPokemonDB::Natures[EPokemonNature::Adamant] = FPokemonNature(EPokemonNature::Adamant, "Adamant", 1.1f, 1.0f, 0.9f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Bashful] = FPokemonNature(EPokemonNature::Bashful, "Bashful", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Bold] = FPokemonNature(EPokemonNature::Bold, "Bold", 0.9f, 1.1f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Brave] = FPokemonNature(EPokemonNature::Brave, "Brave", 1.1f, 1.0f, 1.0f, 1.0f, 0.9f);
		UPokemonDB::Natures[EPokemonNature::Calm] = FPokemonNature(EPokemonNature::Calm, "Calm", 0.9f, 1.0f, 1.0f, 1.1f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Careful] = FPokemonNature(EPokemonNature::Careful, "Careful", 1.0f, 1.0f, 0.9f, 1.1f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Docile] = FPokemonNature(EPokemonNature::Docile, "Docile", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Gentle] = FPokemonNature(EPokemonNature::Gentle, "Gentle", 1.0f, 0.9f, 1.0f, 1.1f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Hardy] = FPokemonNature(EPokemonNature::Hardy, "Hardy", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Hasty] = FPokemonNature(EPokemonNature::Hasty, "Hasty", 1.0f, 0.9f, 1.0f, 1.0f, 1.1f);
		UPokemonDB::Natures[EPokemonNature::Impish] = FPokemonNature(EPokemonNature::Impish, "Impish", 1.0f, 1.1f, 0.9f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Jolly] = FPokemonNature(EPokemonNature::Jolly, "Jolly", 1.0f, 1.0f, 0.9f, 1.0f, 1.1f);
		UPokemonDB::Natures[EPokemonNature::Lax] = FPokemonNature(EPokemonNature::Lax, "Lax", 1.0f, 1.1f, 1.0f, 0.9f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Lonely] = FPokemonNature(EPokemonNature::Lonely, "Lonely", 1.1f, 0.9f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Mild] = FPokemonNature(EPokemonNature::Mild, "Mild", 1.0f, 0.9f, 1.1f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Modest] = FPokemonNature(EPokemonNature::Modest, "Modest", 0.9f, 1.0f, 1.1f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Naive] = FPokemonNature(EPokemonNature::Naive, "Naive", 1.0f, 1.0f, 1.0f, 0.9f, 1.1f);
		UPokemonDB::Natures[EPokemonNature::Naughty] = FPokemonNature(EPokemonNature::Naughty, "Naughty", 1.1f, 1.0f, 1.0f, 0.9f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Quiet] = FPokemonNature(EPokemonNature::Quiet, "Quiet", 1.0f, 1.0f, 1.1f, 1.0f, 0.9f);
		UPokemonDB::Natures[EPokemonNature::Quirky] = FPokemonNature(EPokemonNature::Quirky, "Quirky", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Rash] = FPokemonNature(EPokemonNature::Rash, "Rash", 1.0f, 1.0f, 1.1f, 0.9f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Relaxed] = FPokemonNature(EPokemonNature::Relaxed, "Relaxed", 1.0f, 1.1f, 1.0f, 1.0f, 0.9f);
		UPokemonDB::Natures[EPokemonNature::Sassy] = FPokemonNature(EPokemonNature::Sassy, "Sassy", 1.0f, 1.0f, 1.0f, 1.1f, 0.9f);
		UPokemonDB::Natures[EPokemonNature::Serious] = FPokemonNature(EPokemonNature::Serious, "Serious", 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		UPokemonDB::Natures[EPokemonNature::Timid] = FPokemonNature(EPokemonNature::Timid, "Timid", 0.9f, 1.0f, 1.0f, 1.0f, 1.1f);

		UPokemonDB::Abilities[EPokemonAbility::Overgrow] = FPokemonAbility(EPokemonAbility::Overgrow, "Overgrow", "Ups GRASS moves in a pinch.");
		UPokemonDB::Abilities[EPokemonAbility::Blaze] = FPokemonAbility(EPokemonAbility::Blaze, "Blaze", "Ups FIRE moves in a pinch.");
		UPokemonDB::Abilities[EPokemonAbility::Torrent] = FPokemonAbility(EPokemonAbility::Torrent, "Torrent", "Ups WATER moves in a pinch.");
		UPokemonDB::Abilities[EPokemonAbility::ShieldDust] = FPokemonAbility(EPokemonAbility::ShieldDust, "ShieldDust", "Prevents added effects.");
		UPokemonDB::Abilities[EPokemonAbility::ShedSkin] = FPokemonAbility(EPokemonAbility::ShedSkin, "ShedSkin", "Heals the body by shedding.");
		UPokemonDB::Abilities[EPokemonAbility::CompoundEyes] = FPokemonAbility(EPokemonAbility::CompoundEyes, "CompoundEyes", "Raises accuracy.");
		UPokemonDB::Abilities[EPokemonAbility::Swarm] = FPokemonAbility(EPokemonAbility::Swarm, "Swarm", "Ups BUG moves in a pinch.");
		UPokemonDB::Abilities[EPokemonAbility::KeenEye] = FPokemonAbility(EPokemonAbility::KeenEye, "KeenEye", "Prevents loss of accuracy.");
		UPokemonDB::Abilities[EPokemonAbility::RunAway] = FPokemonAbility(EPokemonAbility::RunAway, "RunAway", "Makes escaping easier.");
		UPokemonDB::Abilities[EPokemonAbility::Guts] = FPokemonAbility(EPokemonAbility::Guts, "Guts", "Ups ATTACK if suffering.");
		UPokemonDB::Abilities[EPokemonAbility::Intimidate] = FPokemonAbility(EPokemonAbility::Intimidate, "Intimidate", "Lowers the foe¡¯s ATTACK.");
		UPokemonDB::Abilities[EPokemonAbility::Static] = FPokemonAbility(EPokemonAbility::Static, "Static", "Paralyzes on contact.");
		UPokemonDB::Abilities[EPokemonAbility::SandVeil] = FPokemonAbility(EPokemonAbility::SandVeil, "SandVeil", "Ups evasion in a sandstorm.");
		UPokemonDB::Abilities[EPokemonAbility::PoisonPoint] = FPokemonAbility(EPokemonAbility::PoisonPoint, "PoisonPoint", "Poisons foe on contact.");

		UPokemonDB::Types[EPokemonType::Normal] = FPokemonType(EPokemonType::Normal, "TypeNormal.png");
		UPokemonDB::Types[EPokemonType::Fighting] = FPokemonType(EPokemonType::Fighting, "TypeFighting.png");
		UPokemonDB::Types[EPokemonType::Flying] = FPokemonType(EPokemonType::Flying, "TypeFlying.png");
		UPokemonDB::Types[EPokemonType::Poison] = FPokemonType(EPokemonType::Poison, "TypePoison.png");
		UPokemonDB::Types[EPokemonType::Ground] = FPokemonType(EPokemonType::Ground, "TypeGround.png");
		UPokemonDB::Types[EPokemonType::Rock] = FPokemonType(EPokemonType::Rock, "TypeRock.png");
		UPokemonDB::Types[EPokemonType::Bug] = FPokemonType(EPokemonType::Bug, "TypeBug.png");
		UPokemonDB::Types[EPokemonType::Ghost] = FPokemonType(EPokemonType::Ghost, "TypeGhost.png");
		UPokemonDB::Types[EPokemonType::Steel] = FPokemonType(EPokemonType::Steel, "TypeSteel.png");
		UPokemonDB::Types[EPokemonType::Fire] = FPokemonType(EPokemonType::Fire, "TypeFire.png");
		UPokemonDB::Types[EPokemonType::Water] = FPokemonType(EPokemonType::Water, "TypeWater.png");
		UPokemonDB::Types[EPokemonType::Grass] = FPokemonType(EPokemonType::Grass, "TypeGrass.png");
		UPokemonDB::Types[EPokemonType::Electric] = FPokemonType(EPokemonType::Electric, "TypeElectric.png");
		UPokemonDB::Types[EPokemonType::Psychic] = FPokemonType(EPokemonType::Psychic, "TypePsychic.png");
		UPokemonDB::Types[EPokemonType::Ice] = FPokemonType(EPokemonType::Ice, "TypeIce.png");
		UPokemonDB::Types[EPokemonType::Dragon] = FPokemonType(EPokemonType::Dragon, "TypeDragon.png");
		UPokemonDB::Types[EPokemonType::Dark] = FPokemonType(EPokemonType::Dark, "TypeDark.png");
		UPokemonDB::Types[EPokemonType::Fairy] = FPokemonType(EPokemonType::Fairy, "TypeFairy.png");

		UPokemonDB::Genders[EPokemonGender::Male] = FPokemonGender(
			EPokemonGender::Male, RN::GenderMarkMale, RN::BigGenderMarkMale
		);
		UPokemonDB::Genders[EPokemonGender::Female] = FPokemonGender(
			EPokemonGender::Female, RN::GenderMarkFemale, RN::BigGenderMarkFemale
		);
	}
};

PokemonDBInitiator Init;