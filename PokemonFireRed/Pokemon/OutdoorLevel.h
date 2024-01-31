#pragma once
#include <EngineCore/Level.h>

// Ό³Έν :
class UOutdoorLevel : public ULevel
{
public:
	// constructor destructor
	UOutdoorLevel();
	~UOutdoorLevel();

	// delete Function
	UOutdoorLevel(const UOutdoorLevel& _Other) = delete;
	UOutdoorLevel(UOutdoorLevel&& _Other) noexcept = delete;
	UOutdoorLevel& operator=(const UOutdoorLevel& _Other) = delete;
	UOutdoorLevel& operator=(UOutdoorLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:
	
};

