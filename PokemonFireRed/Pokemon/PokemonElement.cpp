#include "PokemonElement.h"
#include "PokemonUtil.h"

APokemonElement::APokemonElement()
{
}

APokemonElement::~APokemonElement()
{
}

void APokemonElement::SetPokemon(const UPokemon& _Pokemon)
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

	EPokedexNo PokedexNo = _Pokemon.GetPokedexNo();
	std::string AnimName = Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(PokedexNo);
	Renderer->ChangeAnimation(AnimName);
}

