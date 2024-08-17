#pragma once

#include <map>
#include <list>
#include <functional>
#include <EngineCore/Actor.h>
#include "PokemonMath.h"
#include "EventManager.h"
#include "EventTarget.h"
#include "EventEnums.h"
#include "EventStream.h"

class UEventStream;
class APlayerCharacter;

// 플레이어와의 상호 작용을 통해 이벤트를 발생시키는 액터
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

	void RegisterEvent(EEventTriggerAction _Action, const std::function<bool()>& _Cond, const UEventStream& _Stream);
	bool TriggerEvent(EEventTriggerAction _Action);

	virtual void RegisterPredefinedEvent() {};

protected:
	void UnregisterEvents();

private:
	struct FCondAndStream
	{
	public:
		FCondAndStream(const std::function<bool()>& _Cond, const UEventStream* _Stream)
			: Cond(_Cond), Stream(_Stream) {};

		std::function<bool()> Cond;
		const UEventStream* Stream = nullptr;
	};

	// AllProcessors[액션] = { (조건, 스트림), ... }
	std::map<EEventTriggerAction, std::list<FCondAndStream>> AllStreams;
};

