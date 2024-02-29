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
		Renderer = CreateImageRenderer(RenderingOrder);
		Renderer->CameraEffectOff();
		
		switch (ElementType)
		{
		case EPokemonElementType::MiniMove:
			Renderer->SetImage(RN::PokemonMiniPlaceHolder);
			UPokemonUtil::CreatePokemonMiniMoveAnimations(Renderer);
			break;
		case EPokemonElementType::MiniStop:
			Renderer->SetImage(RN::PokemonMiniPlaceHolder);
			UPokemonUtil::CreatePokemonMiniStopAnimations(Renderer);
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

		OriginalRenderScale = UPokemonUtil::GetRenderScale(Renderer);
	}

	EPokedexNo PokedexNo = _Pokemon->GetPokedexNo();
	std::string AnimPrefix; 
	switch (ElementType)
	{
	case EPokemonElementType::MiniMove:
		AnimPrefix = Global::PokemonMiniMovePrefix;
		break;
	case EPokemonElementType::MiniStop:
		AnimPrefix = Global::PokemonMiniStopPrefix;
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

	Renderer->ChangeAnimation(AnimPrefix + _Pokemon->GetSpeciesNameA());
}

void APokemonElement::SetPokemon(const UPokemon& _Pokemon)
{
	SetPokemon(&_Pokemon);
}

