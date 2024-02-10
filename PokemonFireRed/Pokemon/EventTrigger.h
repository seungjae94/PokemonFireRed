#pragma once
#include <functional>
#include <EngineCore/Actor.h>
#include "PokemonMath.h"
#include "EventManager.h"

class APlayer;

// �÷��̾���� ��ȣ �ۿ��� ���� �̺�Ʈ�� �߻���Ű�� ����
class AEventTrigger : public AActor
{
	friend UPokemonLevel;
public:
	// constructor destructor
	AEventTrigger();
	~AEventTrigger();

	// delete Function
	AEventTrigger(const AEventTrigger& _Other) = delete;
	AEventTrigger(AEventTrigger&& _Other) noexcept = delete;
	AEventTrigger& operator=(const AEventTrigger& _Other) = delete;
	AEventTrigger& operator=(AEventTrigger&& _Other) noexcept = delete;
protected:
	virtual void Initialize() {};
	virtual void RegisterEvents() {};
private:
};

