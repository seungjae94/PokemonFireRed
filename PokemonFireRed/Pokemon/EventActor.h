#pragma once
#include <functional>
#include "PokemonMath.h"
#include "MapLevel.h"
#include "Movable.h"
#include "EventDelegate.h"

class APlayer;

// �׻� �÷��̾ ���� ���� ��ȭ�� �����ϸ鼭 �ڽ��� ���¸� �����ϰ� �÷��̾�� ��ȣ �ۿ��ϴ� ����
class AEventActor : public AMovable
{
public:
	using Event = std::function<bool(AEventActor*, float)>;
public:
	// constructor destructor
	AEventActor();
	~AEventActor();

	// delete Function
	AEventActor(const AEventActor& _Other) = delete;
	AEventActor(AEventActor&& _Other) noexcept = delete;
	AEventActor& operator=(const AEventActor& _Other) = delete;
	AEventActor& operator=(AEventActor&& _Other) noexcept = delete;

	void SetPlayer(APlayer* _Player)
	{
		Player = _Player;
	}

	APlayer* GetPlayer()
	{
		return Player;
	}

	virtual void TriggerEvent() {};

	void SetTilePoint(const FTileVector& _Point);

	void SetMapLevel(UMapLevel* _MapLevel)
	{
		MapLevel = _MapLevel;
	}

	void RegisterEvent(Event _Event)
	{
		AllEvents.push_back(_Event);
	}

protected:
	APlayer* Player;
	bool IsTriggered = false;
	void Tick(float _DeltaTime) override;

	UEventDelegate EventDelegate;
private:
	UMapLevel* MapLevel;

	int CurEventIndex = 0;
	std::vector<Event> AllEvents;

	//int EventSetType = 0;
	//std::map<int, Event>> AllEvents;
};

