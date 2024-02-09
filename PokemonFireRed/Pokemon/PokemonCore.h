#pragma once
#include <EngineCore/EngineCore.h>

// Ό³Έν :
class PokemonCore : public UEngineCore
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

	virtual void Hello() {};

protected:
	void BeginPlay() override;
private:

};

