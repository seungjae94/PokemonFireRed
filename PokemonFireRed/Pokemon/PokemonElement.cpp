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
		UPokemonUtil::CreatePokemonMiniAnimations(Renderer);
		FVector RenderScale = Global::MiniPokemonRenderScale;
		FVector Pos = RenderScale.Half2D();
		Renderer->SetTransform({ Pos, RenderScale });
	}

	EPokedexNo PokedexNo = _Pokemon.GetPokedexNo();
	std::string AnimName = Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(PokedexNo);
	Renderer->ChangeAnimation(AnimName);
}

