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

	// ���� Ŭ�������� �ʹ� ���� ����� �� ���Ƽ� protected�� ����
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

