#pragma once
#include "PokemonLevel.h"

class UTutorialLevel : public UPokemonLevel
{
public:
	// constructor destructor
	UTutorialLevel();
	~UTutorialLevel();

	// delete Function
	UTutorialLevel(const UTutorialLevel& _Other) = delete;
	UTutorialLevel(UTutorialLevel&& _Other) noexcept = delete;
	UTutorialLevel& operator=(const UTutorialLevel& _Other) = delete;
	UTutorialLevel& operator=(UTutorialLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:

};

