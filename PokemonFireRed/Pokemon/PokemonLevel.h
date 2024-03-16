#pragma once
#include <EngineCore/Level.h>
#include "EventManager.h"
#include "EventTrigger.h"
#include "Player.h"
#include "MenuCanvas.h"
#include "DialogueWindow.h"
#include "FadeLevelChanger.h"
#include "FadeCanvas.h"

// 포켓몬 게임의 모든 레벨이 공통적으로 수행해야 하는 Tick 동작을 정의하는 클래스
// - 최초 개발 의도: 이벤트 매니저의 Tick을 돌리기 위해 생성
class UPokemonLevel : public ULevel
{
public:
	// constructor destructor
	UPokemonLevel();
	~UPokemonLevel();

	// delete Function
	UPokemonLevel(const UPokemonLevel& _Other) = delete;
	UPokemonLevel(UPokemonLevel&& _Other) noexcept = delete;
	UPokemonLevel& operator=(const UPokemonLevel& _Other) = delete;
	UPokemonLevel& operator=(UPokemonLevel&& _Other) noexcept = delete;

	template <typename EventTargetType>
	EventTargetType* SpawnEventTarget(UEventTargetSetting _Setting)
	{
		EventTargetType* EventTarget = SpawnActor<EventTargetType>();
		UEventManager::AddTarget(EventTarget, _Setting);
		return EventTarget;
	}

	APlayer* SpawnPlayer(const FTileVector& _Point)
	{
		APlayer* Player = SpawnActor<APlayer>();
		UEventManager::AddPlayer(Player, _Point);
		return Player;
	}

	template <typename CanvasType>
	CanvasType* SpawnCommonCanvas(std::string_view _Name)
	{
		CanvasType* UIElement = SpawnActor<CanvasType>();
		UEventManager::AddCommonCanvas(UIElement, _Name);
		return UIElement;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(UEventTargetSetting _Setting)
	{
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTarget(EventTrigger, _Setting);
		UEventManager::AddTrigger(EventTrigger, _Setting);
		return EventTrigger;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(std::string_view _Name)
	{
		UEventTargetSetting Init;
		Init.SetName(_Name);
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTarget(EventTrigger, Init);
		UEventManager::AddTrigger(EventTrigger, Init);
		return EventTrigger;
	}

protected:
	void BeginPlay() override;
	void LevelStart(ULevel* _PrevLevel) override;
	void LevelEnd(ULevel* _NextLevel) override;
	void Tick(float _DeltaTime) override;
private:
	AFadeCanvas* FadeCanvas = nullptr;

	static int DamageCheatStage;
	static int ExpCheatStage;
	static int CatchCheatStage;
	static int EncounterCheatStage;

	static void ApplyDamageCheat();
	static void ApplyExpCheat();
	static void ApplyCatchCheat();
	static void ApplyEncounterCheat();
};

