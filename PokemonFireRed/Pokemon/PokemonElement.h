#pragma once
#include "UIElement.h"
#include "Pokemon.h"

enum class EPokemonElementType
{
	Mini,
	Front,
	Back
};

class APokemonElement : public AUIElement
{
	friend class APage;
public:
	// constructor destructor
	APokemonElement();
	~APokemonElement();

	// delete Function
	APokemonElement(const APokemonElement& _Other) = delete;
	APokemonElement(APokemonElement&& _Other) noexcept = delete;
	APokemonElement& operator=(const APokemonElement& _Other) = delete;
	APokemonElement& operator=(APokemonElement&& _Other) noexcept = delete;

	void SetPokemon(const UPokemon& _Pokemon);

protected:

private:
	UImageRenderer* Renderer = nullptr;
	EPokemonElementType Type = EPokemonElementType::Mini;
};

