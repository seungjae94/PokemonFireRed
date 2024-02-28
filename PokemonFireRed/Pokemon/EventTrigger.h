#pragma once
#include <functional>
#include <EngineCore/Actor.h>
#include "PokemonMath.h"
#include "EventManager.h"
#include "EventTarget.h"
#include "EventStream.h"

class APlayer;

// �÷��̾���� ��ȣ �ۿ��� ���� �̺�Ʈ�� �߻���Ű�� ����
class AEventTrigger : public AEventTarget
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

	virtual void RegisterPredefinedEvent() {};
protected:
	UEventCondition Cond;

	virtual void UnregisterEvent() {
		UEventManager::UnregisterEvent(this, Cond);
	};
private:
};

