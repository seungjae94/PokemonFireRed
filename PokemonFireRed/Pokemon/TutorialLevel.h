#pragma once
#include "PokemonLevel.h"
#include <EnginePlatform/EngineSound.h>

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
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;
private:
	ATutorialLevelManager* Manager = nullptr;
	AEventTrigger* MapLevelChanger = nullptr;
	UEngineSoundPlayer BgmPlayer;
};

