#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"

class APlayer;
class AMap;
class AMenuWindow;
class ADialogueWindow;
class AEventTrigger;

class UMapLevel : public UPokemonLevel
{
	friend AEventTrigger;
public:
	// constructor destructor
	UMapLevel();
	~UMapLevel();

	// delete Function
	UMapLevel(const UMapLevel& _Other) = delete;
	UMapLevel(UMapLevel&& _Other) noexcept = delete;
	UMapLevel& operator=(const UMapLevel& _Other) = delete;
	UMapLevel& operator=(UMapLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 하위 클래스에서 너무 많이 사용할 것 같아서 protected로 설정
	AMap* Map = nullptr;
	APlayer* Player = nullptr;
	AMenuWindow* MenuWindow = nullptr;
	ADialogueWindow* DialogueWindow = nullptr;

	AEventTrigger* FadeInTrigger = nullptr;

	UEngineDirectory CurDir;

	void LevelStart(ULevel* _PrevLevel) override;

private:
	void LoadCharacterResources();
};

