#pragma once
#include "UIElement.h"
#include "Pokemon.h"

enum class EPokemonElementType
{
	MiniMove,
	MiniStop,
	Front,
	Back
};

class APokemonElement : public AUIElement
{
	friend class ACanvas;
public:
	// constructor destructor
	APokemonElement();
	~APokemonElement();

	// delete Function
	APokemonElement(const APokemonElement& _Other) = delete;
	APokemonElement(APokemonElement&& _Other) noexcept = delete;
	APokemonElement& operator=(const APokemonElement& _Other) = delete;
	APokemonElement& operator=(APokemonElement&& _Other) noexcept = delete;

	void SetPokemon(const UPokemon* _Pokemon);
	void SetPokemon(const UPokemon& _Pokemon);

	void SetScaleFactor(float _Factor)
	{
		Renderer->SetScale(OriginalRenderScale * _Factor);
	}

protected:

private:
	FVector OriginalRenderScale;
	UImageRenderer* Renderer = nullptr;
	EPokemonElementType Type = EPokemonElementType::MiniMove;
};

