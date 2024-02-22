#pragma once
#include "PokemonLevel.h"

class ATutorialLevelManager;

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

	void LevelChange();

protected:
	void BeginPlay() override;
private:
	ATutorialLevelManager* Manager = nullptr;
	AEventTrigger* MapLevelChanger = nullptr;
};

