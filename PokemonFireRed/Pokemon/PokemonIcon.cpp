#include "PokemonIcon.h"

APokemonIcon::APokemonIcon()
{
}

APokemonIcon::~APokemonIcon()
{
}

void APokemonIcon::SetPokemon(const UPokemon& _Pokemon)
{
	if (nullptr == Renderer)
	{
		Renderer = CreateImageRenderer(ERenderingOrder::Upper2UI);
		Renderer->CameraEffectOff();
		UPokemonUtil::CreateMiniPokemonAnimations(Renderer);
		FVector RenderScale = Global::MiniPokemonRenderScale;
		FVector Pos = RenderScale.Half2D();
		Renderer->SetTransform({ Pos, RenderScale });
	}

	EPokedexNo PokedexNo = _Pokemon.GetPokedexNo();
	std::string AnimName = Global::PokemonMiniPrefix + UPokemon::GetSpeciesName(PokedexNo);
	Renderer->ChangeAnimation(AnimName);
}

