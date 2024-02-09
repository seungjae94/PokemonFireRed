#pragma once
#include <EngineCore/Level.h>
#include "EventManager.h"
#include "EventTrigger.h"
#include "Player.h"

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
	EventTargetType* SpawnEventTarget(std::string_view _Name, const FTileVector& _Point)
	{
		EventTargetType* EventTarget = SpawnActor<EventTargetType>();
		UEventManager::AddTarget(EventTarget, UEngineString::ToUpper(_Name), _Point);
		return EventTarget;
	}

	APlayer* SpawnPlayer(const FTileVector& _Point)
	{
		APlayer* Player = SpawnActor<APlayer>();
		UEventManager::AddPlayer(Player, _Point);
		return Player;
	}

	template <typename EventTriggerType>
	EventTriggerType* SpawnEventTrigger(std::string_view _Name, const FTileVector& _Point)
	{
		EventTriggerType* EventTrigger = SpawnActor<EventTriggerType>();
		UEventManager::AddTrigger(EventTrigger, UEngineString::ToUpper(_Name), _Point);

		AEventTrigger* UpEventTrigger = EventTrigger;
		UpEventTrigger->RegisterEvents();

		return EventTrigger;
	}

	template <typename EventTriggerTargetType>
	EventTriggerTargetType SpawnEventTriggerTarget(std::string_view _Name, const FTileVector& _Point)
	{
		EventTriggerTargetType* EventTriggerTarget = SpawnActor<EventTriggerTargetType>();
		UEventManager::AddTarget(EventTriggerTarget, UEngineString::ToUpper(_Name), _Point);
		UEventManager::AddTrigger(EventTriggerTarget, UEngineString::ToUpper(_Name), _Point);
		
		AEventTrigger* UpEventTriggerTarget = EventTriggerTarget;
		UpEventTriggerTarget->RegisterEvents();

		return EventTriggerTarget;
	}

protected:
	void Tick(float _DeltaTime) override;
private:

};

