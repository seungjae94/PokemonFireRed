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
		std::string FilePath = CurDir.AppendPath("Nature.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonNature Nature = FPokemonNature();
			Nature.Id = static_cast<EPokemonNature>(std::stoi(Line[0]));
			Nature.Name = Line[1];
			Nature.NAtk = std::stof(Line[2]);
			Nature.NDef = std::stof(Line[3]);
			Nature.NSpAtk = std::stof(Line[4]);
			Nature.NSpDef = std::stof(Line[5]);
			Nature.NSpeed = std::stof(Line[6]);
			UPokemonDB::Natures[Nature.Id] = Nature;
		}
	}
	void GenerateAbilities()
	{
		std::string FilePath = CurDir.AppendPath("Ability.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonAbility Ability = FPokemonAbility();
			Ability.Id = static_cast<EPokemonAbility>(std::stoi(Line[0]));
			Ability.Name = Line[1];
			Ability.Explain = Line[2];
			UPokemonDB::Abilities[Ability.Id] = Ability;
		}
	}
	void GenerateTypes()
	{
		std::string FilePath = CurDir.AppendPath("Type.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonType Type = FPokemonType();
			Type.Id = static_cast<EPokemonType>(std::stoi(Line[0]));
			Type.ImageName = Line[1];

			for (int i = 2; i <= 18; ++i)
			{
				EPokemonType EnemyType = static_cast<EPokemonType>(i - 1);
				Type.EffectTo[EnemyType] = std::stof(Line[i]);
			}

			UPokemonDB::Types[Type.Id] = Type;
		}
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