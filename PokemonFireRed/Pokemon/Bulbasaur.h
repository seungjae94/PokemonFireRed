#pragma once
#include "Pokemon.h"

class UBulbasaur : public UPokemon
{
public:
	// constructor destructor
	UBulbasaur();
	~UBulbasaur();

	// delete Function
	UBulbasaur(const UBulbasaur& _Other) = delete;
	UBulbasaur(UBulbasaur&& _Other) noexcept = delete;
	UBulbasaur& operator=(const UBulbasaur& _Other) = delete;
	UBulbasaur& operator=(UBulbasaur&& _Other) noexcept = delete;

protected:

private:

};

