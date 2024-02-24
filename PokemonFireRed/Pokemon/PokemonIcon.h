#pragma once
#include "UIElement.h"
#include "Pokemon.h"

class APokemonIcon : public AUIElement
{
	friend class APage;
public:
	// constructor destructor
	APokemonIcon();
	~APokemonIcon();

	// delete Function
	APokemonIcon(const APokemonIcon& _Other) = delete;
	APokemonIcon(APokemonIcon&& _Other) noexcept = delete;
	APokemonIcon& operator=(const APokemonIcon& _Other) = delete;
	APokemonIcon& operator=(APokemonIcon&& _Other) noexcept = delete;

	void SetPokemon(const UPokemon& _Pokemon);

protected:

private:
	UImageRenderer* Renderer = nullptr;
};

