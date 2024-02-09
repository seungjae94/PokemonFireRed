#pragma once
#include "EventActor.h"

class AWarp : public AEventActor
{
public:
	// constructor destructor
	AWarp();
	~AWarp();

	// delete Function
	AWarp(const AWarp& _Other) = delete;
	AWarp(AWarp&& _Other) noexcept = delete;
	AWarp& operator=(const AWarp& _Other) = delete;
	AWarp& operator=(AWarp&& _Other) noexcept = delete;

	void SetTargetLevelName(std::string_view _LevelName)
	{
		TargetMapName = _LevelName;
	}

	void SetTargetPoint(const FTileVector& _Point)
	{
		TargetPoint = _Point;
	}

	static std::string GetCurTargetMapName()
	{
		return CurTargetMapName;
	}

	static FTileVector GetCurTargetPoint()
	{
		return CurTargetPoint;
	}

	static FTileVector GetCurTargetDirection()
	{
		return CurTargetDirection;
	}

protected:
	void TriggerEvent() override;
	void BeginPlay() override;
private:
	UImageRenderer* Renderer = nullptr;
	std::string TargetMapName;
	FTileVector TargetPoint;

	static std::string CurTargetMapName;
	static FTileVector CurTargetPoint;
	static FTileVector CurTargetDirection;

	bool Event1(float _DeltaTime);
	bool Event2(float _DeltaTime);
	bool Event3(float _DeltaTime);
	bool Event4(float _DeltaTime);
};

