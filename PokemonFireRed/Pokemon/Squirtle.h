#pragma once
#include "Pokemon.h"

class USquirtle : public UPokemon
{
public:
	// constructor destructor
	USquirtle();
	~USquirtle();

	// delete Function
	USquirtle(const USquirtle& _Other) = delete;
	USquirtle(USquirtle&& _Other) noexcept = delete;
	USquirtle& operator=(const USquirtle& _Other) = delete;
	USquirtle& operator=(USquirtle&& _Other) noexcept = delete;

protected:

private:

};

