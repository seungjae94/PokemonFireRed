#include "PokemonDB.h"
//#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include "CsvReader.h"

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
		CurDir.MoveToSearchChild("GameData");
		GeneratePokemons();
		GenerateMoves();
		GenerateAbilities();
		GenerateNatures();
		GenerateTypes();
		GenerateGenders();
		GenerateGrassInfos();
	}
	
	void GeneratePokemons() {
		// ·¹º§¾÷ ½ºÅ³ Ãß°¡
		std::map<EPokedexNo, std::map<int, std::vector<EPokemonMove>>> LevelUpMoveMap;

		std::string LevelUpMoveFilePath = CurDir.AppendPath("LevelUpMove.csv");
		UCsvReader LevelUpMoveFileReader = UCsvReader(LevelUpMoveFilePath);
		std::vector<std::vector<std::string>> LevelUpMoveLines = LevelUpMoveFileReader.ReadLines();

		for (std::vector<std::string>& Line : LevelUpMoveLines)
		{
			EPokedexNo Id = static_cast<EPokedexNo>(std::stoi(Line[1]));
			int Level = std::stoi(Line[2]);
			EPokemonMove MoveId = static_cast<EPokemonMove>(std::stoi(Line[3]));

			LevelUpMoveMap[Id][Level].push_back(MoveId);
		}

		// Æ÷ÄÏ¸ó Á¾Á· Ãß°¡
		std::string PokemonFilePath = CurDir.AppendPath("Pokemon.csv");
		UCsvReader PokemonFileReader = UCsvReader(PokemonFilePath);
		std::vector<std::vector<std::string>> PokemonLines = PokemonFileReader.ReadLines();

		for (std::vector<std::string>& Line : PokemonLines)
		{
			FPokemonSpecies Species = FPokemonSpecies();
			Species.Id = static_cast<EPokedexNo>(std::stoi(Line[0]));
			Species.Name = Line[1];
			Species.BHp = std::stoi(Line[2]);
			Species.BAtk = std::stoi(Line[3]);
			Species.BDef = std::stoi(Line[4]);
			Species.BSpAtk = std::stoi(Line[5]);
			Species.BSpDef = std::stoi(Line[6]);
			Species.BSpeed = std::stoi(Line[7]);
			Species.YHp = std::stoi(Line[8]);
			Species.YAtk = std::stoi(Line[9]);
			Species.YDef = std::stoi(Line[10]);
			Species.YSpAtk = std::stoi(Line[11]);
			Species.YSpDef = std::stoi(Line[12]);
			Species.YSpeed = std::stoi(Line[13]);
			Species.ExpGroup = static_cast<EExperienceGroup>(std::stoi(Line[14]));
			Species.MaleRatio = std::stof(Line[15]);
			Species.CatchRate = std::stoi(Line[16]);
			Species.Friendship = std::stoi(Line[17]);
			
			for (int i = 18; i <= 19; ++i)
			{
				int LineInt = std::stoi(Line[i]);

				if (0 == LineInt)
				{
					continue;
				}

				Species.TypeIds.push_back(static_cast<EPokemonType>(LineInt));
			}

			for (int i = 20; i <= 22; ++i)
			{
				int LineInt = std::stoi(Line[i]);

				if (0 == LineInt)
				{
					continue;
				}

				Species.AbilityCandidateIds.push_back(static_cast<EPokemonAbility>(LineInt));
			}

			for (const std::pair<int, std::vector<EPokemonMove>>& Pair : LevelUpMoveMap[Species.Id])
			{
				int Level = Pair.first;
				for (EPokemonMove MoveId : Pair.second)
				{
					Species.AddLevelUpMove(Level, MoveId);
				}
			}

			UPokemonDB::Species[Species.Id] = Species;
			UPokemonDB::ImplementedSpeciesNo.push_back(Species.Id);
		}
	}
	void GenerateMoves() {
		std::string FilePath = CurDir.AppendPath("Move.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonMove Move = FPokemonMove();
			Move.Id = static_cast<EPokemonMove>(std::stoi(Line[0]));
			Move.Name = Line[1];
			Move.Explain = Line[2];
			Move.Type = static_cast<EPokemonType>(std::stoi(Line[3]));
			Move.PP = std::stoi(Line[4]);
			Move.BasePower = std::stoi(Line[5]);
			Move.Accuracy = std::stoi(Line[6]);
			UPokemonDB::Moves[Move.Id] = Move;
		}
	}
	void GenerateNatures()
	{
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
	}
	void GenerateAbilities()
	{
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
	}
	void GenerateTypes()
	{
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
	}
	void GenerateGenders()
	{
		UPokemonDB::Genders[EPokemonGender::Male] = FPokemonGender(
			EPokemonGender::Male, RN::GenderMarkMale, RN::BigGenderMarkMale
		);
		UPokemonDB::Genders[EPokemonGender::Female] = FPokemonGender(
			EPokemonGender::Female, RN::GenderMarkFemale, RN::BigGenderMarkFemale
		);
	}
	
	void GenerateGrassInfos()
	{
		//// ExteriorPalletTownLevel
		//// 1¹ø µµ·Î(0): ±¸±¸, ²¿·¿
		//// 22¹ø µµ·Î(1): ²¿·¿, ¸ÁÅ°, ±úºñÂü
		//// 2¹ø µµ·Î(2): ±¸±¸, ²¿·¿, Ä³ÅÍÇÇ, »ÔÃæÀÌ
		//FGrassInfo Info;
		//Info.AddWildPokemonInfo({EPokedexNo::});

		//// »ó·Ï½£
		//// 0: Ä³ÅÍÇÇ, ´Üµ¥±â, »ÔÃæÀÌ, ÇÇÄ«Ãò

	}

	UEngineDirectory CurDir;
};

PokemonDBInitiator Init;