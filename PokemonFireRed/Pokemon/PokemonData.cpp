#include "PokemonData.h"

std::map<EPokedexNo, std::string> UPokemonData::SpeciesNames;
std::map<ENature, std::string> UPokemonData::NatureNames;
std::map<EPokemonType, std::string> UPokemonData::TypeImageNames;
std::map<EAbility, std::string> UPokemonData::AbilityNames;
std::map<EAbility, std::string> UPokemonData::AbilityExplainTexts;
std::list<EPokedexNo> UPokemonData::ImplementedSpeciesNo;

class PokemonDataInitiator
{
public:
	PokemonDataInitiator()
	{
		UPokemonData::SpeciesNames[EPokedexNo::Bulbasaur] = "BULBASAUR";
		UPokemonData::SpeciesNames[EPokedexNo::Charmander] = "CHARMANDER";
		UPokemonData::SpeciesNames[EPokedexNo::Squirtle] = "SQUIRTLE";

		UPokemonData::ImplementedSpeciesNo.push_back(EPokedexNo::Bulbasaur);
		UPokemonData::ImplementedSpeciesNo.push_back(EPokedexNo::Charmander);
		UPokemonData::ImplementedSpeciesNo.push_back(EPokedexNo::Squirtle);

		UPokemonData::NatureNames[ENature::Adamant] = UEngineString::ToUpper("Adamant");
		UPokemonData::NatureNames[ENature::Bashful] = UEngineString::ToUpper("Bashful");
		UPokemonData::NatureNames[ENature::Bold] = UEngineString::ToUpper("Bold");
		UPokemonData::NatureNames[ENature::Brave] = UEngineString::ToUpper("Brave");
		UPokemonData::NatureNames[ENature::Calm] = UEngineString::ToUpper("Calm");
		UPokemonData::NatureNames[ENature::Careful] = UEngineString::ToUpper("Careful");
		UPokemonData::NatureNames[ENature::Docile] = UEngineString::ToUpper("Docile");
		UPokemonData::NatureNames[ENature::Gentle] = UEngineString::ToUpper("Gentle");
		UPokemonData::NatureNames[ENature::Hardy] = UEngineString::ToUpper("Hardy");
		UPokemonData::NatureNames[ENature::Hasty] = UEngineString::ToUpper("Hasty");
		UPokemonData::NatureNames[ENature::Impish] = UEngineString::ToUpper("Impish");
		UPokemonData::NatureNames[ENature::Jolly] = UEngineString::ToUpper("Jolly");
		UPokemonData::NatureNames[ENature::Lax] = UEngineString::ToUpper("Lax");
		UPokemonData::NatureNames[ENature::Lonely] = UEngineString::ToUpper("Lonely");
		UPokemonData::NatureNames[ENature::Mild] = UEngineString::ToUpper("Mild");
		UPokemonData::NatureNames[ENature::Modest] = UEngineString::ToUpper("Modest");
		UPokemonData::NatureNames[ENature::Naive] = UEngineString::ToUpper("Naive");
		UPokemonData::NatureNames[ENature::Naughty] = UEngineString::ToUpper("Naughty");
		UPokemonData::NatureNames[ENature::Quiet] = UEngineString::ToUpper("Quiet");
		UPokemonData::NatureNames[ENature::Quirky] = UEngineString::ToUpper("Quirky");
		UPokemonData::NatureNames[ENature::Rash] = UEngineString::ToUpper("Rash");
		UPokemonData::NatureNames[ENature::Relaxed] = UEngineString::ToUpper("Relaxed");
		UPokemonData::NatureNames[ENature::Sassy] = UEngineString::ToUpper("Sassy");
		UPokemonData::NatureNames[ENature::Serious] = UEngineString::ToUpper("Serious");
		UPokemonData::NatureNames[ENature::Timid] = UEngineString::ToUpper("Timid");

		UPokemonData::TypeImageNames[EPokemonType::Normal] = "TypeNormal.png";
		UPokemonData::TypeImageNames[EPokemonType::Fighting] = "TypeFighting.png";
		UPokemonData::TypeImageNames[EPokemonType::Flying] = "TypeFlying.png";
		UPokemonData::TypeImageNames[EPokemonType::Poison] = "TypePoison.png";
		UPokemonData::TypeImageNames[EPokemonType::Ground] = "TypeGround.png";
		UPokemonData::TypeImageNames[EPokemonType::Rock] = "TypeRock.png";
		UPokemonData::TypeImageNames[EPokemonType::Bug] = "TypeBug.png";
		UPokemonData::TypeImageNames[EPokemonType::Ghost] = "TypeGhost.png";
		UPokemonData::TypeImageNames[EPokemonType::Steel] = "TypeSteel.png";
		UPokemonData::TypeImageNames[EPokemonType::Fire] = "TypeFire.png";
		UPokemonData::TypeImageNames[EPokemonType::Water] = "TypeWater.png";
		UPokemonData::TypeImageNames[EPokemonType::Grass] = "TypeGrass.png";
		UPokemonData::TypeImageNames[EPokemonType::Electric] = "TypeElectric.png";
		UPokemonData::TypeImageNames[EPokemonType::Psychic] = "TypePsychic.png";
		UPokemonData::TypeImageNames[EPokemonType::Ice] = "TypeIce.png";
		UPokemonData::TypeImageNames[EPokemonType::Dragon] = "TypeDragon.png";
		UPokemonData::TypeImageNames[EPokemonType::Dark] = "TypeDark.png";
		UPokemonData::TypeImageNames[EPokemonType::Fairy] = "TypeFairy.png";

		UPokemonData::AbilityNames[EAbility::Overgrow] = "Overgrow";
		UPokemonData::AbilityNames[EAbility::Blaze] = "Blaze";
		UPokemonData::AbilityNames[EAbility::Torrent] = "Torrent";
		UPokemonData::AbilityNames[EAbility::ShieldDust] = "Shield Dust";
		UPokemonData::AbilityNames[EAbility::ShedSkin] = "Shed Skin";
		UPokemonData::AbilityNames[EAbility::CompoundEyes] = "Compound Eyes";
		UPokemonData::AbilityNames[EAbility::Swarm] = "Swarm";
		UPokemonData::AbilityNames[EAbility::KeenEye] = "Keen Eye";
		UPokemonData::AbilityNames[EAbility::RunAway] = "Run Away";
		UPokemonData::AbilityNames[EAbility::Guts] = "Guts";
		UPokemonData::AbilityNames[EAbility::Intimidate] = "Intimidate";
		UPokemonData::AbilityNames[EAbility::Static] = "Static";
		UPokemonData::AbilityNames[EAbility::SandVeil] = "Sand Veil";
		UPokemonData::AbilityNames[EAbility::PoisonPoint] = "Poison Point";

		UPokemonData::AbilityExplainTexts[EAbility::Overgrow] = "Ups GRASS moves in a pinch.";
		UPokemonData::AbilityExplainTexts[EAbility::Blaze] = "Ups FIRE moves in a pinch.";
		UPokemonData::AbilityExplainTexts[EAbility::Torrent] = "Ups WATER moves in a pinch.";
		UPokemonData::AbilityExplainTexts[EAbility::ShieldDust] = "Prevents added effects.";
		UPokemonData::AbilityExplainTexts[EAbility::ShedSkin] = "Heals the body by shedding.";
		UPokemonData::AbilityExplainTexts[EAbility::CompoundEyes] = "Raises accuracy.";
		UPokemonData::AbilityExplainTexts[EAbility::Swarm] = "Ups BUG moves in a pinch.";
		UPokemonData::AbilityExplainTexts[EAbility::KeenEye] = "Prevents loss of accuracy.";
		UPokemonData::AbilityExplainTexts[EAbility::RunAway] = "Makes escaping easier.";
		UPokemonData::AbilityExplainTexts[EAbility::Guts] = "Ups ATTACK if suffering.";
		UPokemonData::AbilityExplainTexts[EAbility::Intimidate] = "Lowers the foe¡¯s ATTACK.";
		UPokemonData::AbilityExplainTexts[EAbility::Static] = "Paralyzes on contact.";
		UPokemonData::AbilityExplainTexts[EAbility::SandVeil] = "Ups evasion in a sandstorm.";
		UPokemonData::AbilityExplainTexts[EAbility::PoisonPoint] = "Poisons foe on contact.";
	}
};

PokemonDataInitiator Init;