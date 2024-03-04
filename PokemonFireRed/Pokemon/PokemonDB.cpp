#include "PokemonDB.h"
#include <EngineBase/EngineDirectory.h>
#include "CsvReader.h"

std::map<std::string, int> UPokemonDB::NameResolver;

std::map<EPokedexNo, FPokemonSpecies> UPokemonDB::Species;
std::map<EPokemonMove, FPokemonMove> UPokemonDB::Moves;
std::map<EPokemonNature, FPokemonNature> UPokemonDB::Natures;
std::map<EPokemonAbility, FPokemonAbility> UPokemonDB::Abilities;
std::map<EPokemonGender, FPokemonGender> UPokemonDB::Genders;
std::map<EPokemonStatus, FPokemonStatus> UPokemonDB::Statuses;
std::map<EPokemonType, FPokemonType> UPokemonDB::Types;
std::list<EPokedexNo> UPokemonDB::ImplementedSpeciesNo;

std::map<std::string, std::map<int, UWildPokemonZone>> UPokemonDB::WildPokemonZones;

class PokemonDBInitiator
{
public:
	PokemonDBInitiator()
	{
		CurDir.MoveToSearchChild("GameData");
		InitNameResolver();
		GeneratePokemons();
		GenerateMoves();
		GenerateAbilities();
		GenerateNatures();
		GenerateTypes();
		GenerateStatuses();
		GenerateGenders();
		GenerateWildPokemonZones();
	}
	
	void InitNameResolver()
	{
		UPokemonDB::NameResolver[""] = 0;

		std::string FilePath = CurDir.AppendPath("NameResolver.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			int IntValue = std::stoi(Line[0]);
			for (int i = 1; i < Line.size(); ++i)
			{
				if (false == Line[i].empty())
				{
					std::string UpperLine = UEngineString::ToUpper(Line[i]);
					UPokemonDB::NameResolver[UpperLine] = IntValue;
					UPokemonDB::NameResolver[UPokemonString::RemoveSpace(UpperLine)] = IntValue;
				}
			}
		}
	}

	void GeneratePokemons() {
		// 레벨업 스킬 추가
		std::map<EPokedexNo, std::map<int, std::vector<EPokemonMove>>> LevelUpMoveMap;

		std::string LevelUpMoveFilePath = CurDir.AppendPath("LevelUpMove.csv");
		UCsvReader LevelUpMoveFileReader = UCsvReader(LevelUpMoveFilePath);
		std::vector<std::vector<std::string>> LevelUpMoveLines = LevelUpMoveFileReader.ReadLines();

		for (std::vector<std::string>& Line : LevelUpMoveLines)
		{
			EPokedexNo Id = static_cast<EPokedexNo>(UPokemonDB::Resolve(Line[0]));
			int Level = std::stoi(Line[1]);
			EPokemonMove MoveId = static_cast<EPokemonMove>(UPokemonDB::Resolve(Line[2]));

			LevelUpMoveMap[Id][Level].push_back(MoveId);
		}

		// 포켓몬 종족 추가
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
			Species.ExpGroup = static_cast<EExperienceGroup>(UPokemonDB::Resolve(Line[14]));
			Species.MaleRatio = std::stof(Line[15]);
			Species.CatchRate = std::stoi(Line[16]);
			Species.Friendship = std::stoi(Line[17]);
			
			for (int i = 18; i <= 19; ++i)
			{
				int LineInt = UPokemonDB::Resolve(Line[i]);

				if (0 == LineInt)
				{
					continue;
				}

				Species.TypeIds.push_back(static_cast<EPokemonType>(LineInt));
			}

			for (int i = 20; i <= 21; ++i)
			{
				int LineInt = UPokemonDB::Resolve(Line[i]);

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
			Move.TypeId = static_cast<EPokemonType>(UPokemonDB::Resolve(Line[3]));
			Move.PP = std::stoi(Line[4]);
			Move.BasePower = std::stoi(Line[5]);
			Move.Accuracy = std::stoi(Line[6]);
			//Move.IsContact = UPokemonDB::Resolve(Line[7]);
			Move.BETarget = static_cast<EMoveEffectTarget>(UPokemonDB::Resolve(Line[8]));
			if (false == Line[9].empty())
			{
				Move.BEStatStageId = static_cast<EStatStageChangeType>(UPokemonDB::Resolve(Line[9]));
			}
			if (false == Line[10].empty())
			{
				Move.BEStatStageValue = std::stoi(Line[10]);
			}
			if (false == Line[11].empty())
			{
				Move.BEStatusId = static_cast<EPokemonStatus>(UPokemonDB::Resolve(Line[11]));
			}

			Move.SETarget = static_cast<EMoveEffectTarget>(UPokemonDB::Resolve(Line[12]));
			if (Line[13] != "")
			{
				Move.SERate = std::stoi(Line[13]);
			}
			if (Line[14] != "")
			{
				Move.SEStatStageId = static_cast<EStatStageChangeType>(UPokemonDB::Resolve(Line[14]));
			}
			if (Line[15] != "")
			{
				Move.SEStatStageValue = std::stoi(Line[15]);
			}
			if (Line[16] != "")
			{
				Move.SEStatusId = static_cast<EPokemonStatus>(UPokemonDB::Resolve(Line[16]));
			}

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
			Type.Name = Line[1];
			Type.ImageName = Line[2];

			for (int i = 3; i <= 19; ++i)
			{
				EPokemonType EnemyType = static_cast<EPokemonType>(i - 2);
				Type.EffectTo[EnemyType] = std::stof(Line[i]);
			}

			UPokemonDB::Types[Type.Id] = Type;
		}
	}

	void GenerateStatuses()
	{
		std::string FilePath = CurDir.AppendPath("Status.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			FPokemonStatus Status = FPokemonStatus();
			Status.Id = static_cast<EPokemonStatus>(std::stoi(Line[0]));
			Status.Name = Line[1];
			Status.ImageName = Line[2];

			UPokemonDB::Statuses[Status.Id] = Status;
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
	
	void GenerateWildPokemonZones()
	{
		std::string FilePath = CurDir.AppendPath("WildPokemonZone.csv");
		UCsvReader Reader = UCsvReader(FilePath);
		std::vector<std::vector<std::string>> Lines = Reader.ReadLines();

		for (std::vector<std::string>& Line : Lines)
		{
			std::string MapName = Line[0];
			int ZoneId = std::stoi(Line[1]);

			UWildPokemonZone& Zone = UPokemonDB::WildPokemonZones[MapName][ZoneId];
			FWildPokemonEncounter Encounter;
			Encounter.Id = static_cast<EPokedexNo>(UPokemonDB::Resolve(Line[2]));
			Encounter.Prop = std::stof(Line[3]);
			Encounter.MinLevel = std::stoi(Line[4]);
			Encounter.MaxLevel = std::stoi(Line[5]);
			Zone.AddWildPokemonEncounter(Encounter);
		}
	}

	UEngineDirectory CurDir;
};

PokemonDBInitiator Init;