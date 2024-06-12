#pragma once
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include "PokemonLevel.h"

class APlayerCharacter;
class AMap;
class AMenuCanvas;
class ADialogueWindow;
class AMapNameCanvas;
class AEventTrigger;
class UEventManager;

class UMapLevel : public UPokemonLevel
{
	friend AEventTrigger;
	friend UEventManager;
public:
	// constructor destructor
	UMapLevel();
	~UMapLevel();

	// delete Function
	UMapLevel(const UMapLevel& _Other) = delete;
	UMapLevel(UMapLevel&& _Other) noexcept = delete;
	UMapLevel& operator=(const UMapLevel& _Other) = delete;
	UMapLevel& operator=(UMapLevel&& _Other) noexcept = delete;

	std::string GetAreaName() const;
	std::string GetAreaBgm() const;

	void SetAreaName(std::string_view _AreaName);
	void SetAreaBgm(std::string_view _AreaBgm);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// ���� Ŭ�������� �ʹ� ���� ����� �� ���Ƽ� protected�� ����
	AMap* Map = nullptr;
	APlayerCharacter* Player = nullptr;
	AEventTrigger* FadeInTrigger = nullptr;

	std::string AreaName = "";
	std::string AreaBgm = "";
	
	UEngineDirectory CurDir;

	void LevelStart(ULevel* _PrevLevel) override;

	void DrawFlowers(const std::vector<FTileVector>& _Points);
	void DrawSeas(const std::vector<FTileVector>& _Points);

private:
	void LoadCharacterResources();
	void LoadObjectResources();
	void LoadTileResources();
};

