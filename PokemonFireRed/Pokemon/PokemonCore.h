#pragma once

// Ό³Έν :
class PokemonCore
{
public:
	// constructor destructor
	PokemonCore();
	~PokemonCore();

	// delete Function
	PokemonCore(const PokemonCore& _Other) = delete;
	PokemonCore(PokemonCore&& _Other) noexcept = delete;
	PokemonCore& operator=(const PokemonCore& _Other) = delete;
	PokemonCore& operator=(PokemonCore&& _Other) noexcept = delete;

protected:

private:

};

