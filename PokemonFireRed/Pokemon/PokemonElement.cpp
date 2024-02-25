#include "PokemonElement.h"
#include "PokemonUtil.h"

APokemonElement::APokemonElement()
{
}

APokemonElement::~APokemonElement()
{
}

void APokemonElement::SetPokemon(const UPokemon* _Pokemon)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		Renderer->CameraEffectOff();
		
		switch (Type)
		{
		case EPokemonElementType::Mini:
			Renderer->SetImage(RN::PokemonMiniPlaceHolder);
			UPokemonUtil::CreatePokemonMiniAnimations(Renderer);
			break;
		case EPokemonElementType::Front:
			Renderer->SetImage(RN::PokemonFrontPlaceHolder);
			UPokemonUtil::CreatePokemonFrontAnimations(Renderer);
			break;
		case EPokemonElementType::Back:
			Renderer->SetImage(RN::PokemonBackPlaceHolder);
			UPokemonUtil::CreatePokemonBackAnimations(Renderer);
			break;
		default:
			break;
		}

		UPokemonUtil::AlignImage(Renderer, PivotType);
	}

	EPokedexNo PokedexNo = _Pokemon->GetPokedexNo();
	std::string AnimPrefix; 
	switch (Type)
	{
	case EPokemonElementType::Mini:
		AnimPrefix = Global::PokemonMiniPrefix;
		break;
	case EPokemonElementType::Front:
		AnimPrefix = Global::PokemonFrontPrefix;
		break;
	case EPokemonElementType::Back:
		AnimPrefix = Global::PokemonBackPrefix;
		break;
	default:
		break;
	}
	Renderer->ChangeAnimation(AnimPrefix + UPokemon::GetSpeciesName(PokedexNo));
}

void APokemonElement::SetPokemon(const UPokemon& _Pokemon)
{
	SetPokemon(&_Pokemon);
}

