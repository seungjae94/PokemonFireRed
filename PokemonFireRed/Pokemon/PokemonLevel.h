#pragma once
#include <EngineCore/Level.h>
#include "EventManager.h"
#include "EventTrigger.h"
#include "Player.h"
#include "MenuWindow.h"

// ���ϸ� ������ ��� ������ ���������� �����ؾ� �ϴ� Tick ������ �����ϴ� Ŭ����
// - ���� ���� �ǵ�: �̺�Ʈ �Ŵ����� Tick�� ������ ���� ����
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
	EventTargetType* SpawnEventTarget(UEventTargetInitialSetting _Setting)
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

	AMenuWindow* SpawnMenuWindow()
	{
		AMenuWindow* MenuWindow = SpawnActor<AMenuWindow>();
		UEventManager::AddMenuWindow(MenuWindow);
		return MenuWindow;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(UEventTargetInitialSetting _Setting)
	{
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTarget(EventTrigger, _Setting);
		UEventManager::AddTrigger(EventTrigger, _Setting);
		
		AEventTrigger* UpEventTriggerTarget = EventTrigger;
		UpEventTriggerTarget->RegisterEvents();

		return EventTrigger;
	}

protected:
	void Tick(float _DeltaTime) override;
private:

};

