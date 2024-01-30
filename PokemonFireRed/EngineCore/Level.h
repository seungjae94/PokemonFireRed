#pragma once
#include <EngineBase/NameObject.h>

// 액터들이 활동할 무대
class ULevel : public UNameObject
{
public:
	// constructor destructor
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	virtual void BeginPlay() 
	{
	}

	virtual void Tick(float _DeltaTime) 
	{
	}

protected:

private:

};

