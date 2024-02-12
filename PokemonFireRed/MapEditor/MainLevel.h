#pragma once
#include <EngineCore/Level.h>

// Ό³Έν :
class UMainLevel : public ULevel
{
public:
	// constructor destructor
	UMainLevel();
	~UMainLevel();

	// delete Function
	UMainLevel(const UMainLevel& _Other) = delete;
	UMainLevel(UMainLevel&& _Other) noexcept = delete;
	UMainLevel& operator=(const UMainLevel& _Other) = delete;
	UMainLevel& operator=(UMainLevel&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
private:

};

