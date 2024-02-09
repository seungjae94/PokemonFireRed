#pragma once
#include <map>

class UPokemonLevel;
class AEventActor;
class UEventProcessor;

// ���� :
class UEventManager
{
	friend UPokemonLevel;
public:
	// delete Function
	UEventManager(const UEventManager& _Other) = delete;
	UEventManager(UEventManager&& _Other) noexcept = delete;
	UEventManager& operator=(const UEventManager& _Other) = delete;
	UEventManager& operator=(UEventManager&& _Other) noexcept = delete;

	static UEventProcessor* CreateEventProcessor(AEventActor* _Caller);

protected:
	// constructor destructor
	UEventManager();
	~UEventManager();
private:
	static std::map<AEventActor*, UEventProcessor*> AllProcessors;
	static void Tick(float _DeltaTime);
};