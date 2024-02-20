#pragma once
#include <EngineBase/EngineDirectory.h>
#include "PokemonLevel.h"

class ATitleLevelManager;

class UTitleLevel : public UPokemonLevel
{
public:
	// constructor destructor
	UTitleLevel();
	~UTitleLevel();

	// delete Function
	UTitleLevel(const UTitleLevel& _Other) = delete;
	UTitleLevel(UTitleLevel&& _Other) noexcept = delete;
	UTitleLevel& operator=(const UTitleLevel& _Other) = delete;
	UTitleLevel& operator=(UTitleLevel&& _Other) noexcept = delete;

	void LevelChange();

protected:
	void BeginPlay() override;
private:
	ATitleLevelManager* Manager;
	AEventTrigger* LevelChanger;
	UEngineDirectory CurDir;
};

