#pragma once
#include <EngineCore/Actor.h>
#include "PokemonMath.h"
#include "MapLevel.h"

class APlayer;

// �׻� �÷��̾ ���� ���� ��ȭ�� �����ϸ鼭 �ڽ��� ���¸� �����ϰ� �÷��̾�� ��ȣ �ۿ��ϴ� ����
class AEventActor : public AActor
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
private:
	UMapLevel* MapLevel;
};

