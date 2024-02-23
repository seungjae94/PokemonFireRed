#pragma once
#include "Pokemon.h"

class UCharmander : public UPokemon
{
public:
	// constructor destructor
	UCharmander();
	~UCharmander();

	// delete Function
	UCharmander(const UCharmander& _Other) = delete;
	UCharmander(UCharmander&& _Other) noexcept = delete;
	UCharmander& operator=(const UCharmander& _Other) = delete;
	UCharmander& operator=(UCharmander&& _Other) noexcept = delete;

protected:

private:

};

