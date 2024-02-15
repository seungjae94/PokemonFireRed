#pragma once
#include "EventTrigger.h"

class AWarp : public AEventTrigger
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

	void SetMoveDirection(const FTileVector& _Direction)
	{
		MoveDirection = _Direction;
	}

	void RegisterPredefinedEvent() override;

protected:
private:
	UImageRenderer* Renderer = nullptr;
	std::string TargetMapName;
	FTileVector TargetPoint;
	FTileVector MoveDirection;

	bool CheckPlayerDirection();
};

