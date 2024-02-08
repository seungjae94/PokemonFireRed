#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/Actor.h>

// Ό³Έν :
class PokemonDebug
{
public:
	// constructor destructor
	PokemonDebug();
	~PokemonDebug();

	// delete Function
	PokemonDebug(const PokemonDebug& _Other) = delete;
	PokemonDebug(PokemonDebug&& _Other) noexcept = delete;
	PokemonDebug& operator=(const PokemonDebug& _Other) = delete;
	PokemonDebug& operator=(PokemonDebug&& _Other) noexcept = delete;

	static void ReportPosition(AActor* _Actor, std::string_view _Name);

protected:

private:

};

