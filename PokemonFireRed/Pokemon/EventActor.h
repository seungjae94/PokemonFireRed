#pragma once
#include <functional>
#include "PokemonMath.h"
#include "MapLevel.h"
#include "Movable.h"
#include "EventHelper.h"
#include "EventProcessor.h"

class APlayer;

// �׻� �÷��̾ ���� ���� ��ȭ�� �����ϸ鼭 �ڽ��� ���¸� �����ϰ� �÷��̾�� ��ȣ �ۿ��ϴ� ����
class AEventActor : public AMovable
{
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

protected:
	APlayer* Player;
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UEventHelper EventDelegate;
	UEventProcessor* EventProcessor;
private:
	UMapLevel* MapLevel;
};

