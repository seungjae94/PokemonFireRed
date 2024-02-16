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

	void SetTargetMapName(std::string_view _MapName)
	{
		TargetMapName = _MapName;
	}

	void SetTargetPoint(const FTileVector& _Point)
	{
		TargetPoint = _Point;
	}

	void SetMoveDirection(const FTileVector& _Direction)
	{
		TargetDirection = _Direction;
	}

protected:
	bool CheckPlayerDirection();
	std::string TargetMapName;
	FTileVector TargetPoint;
	FTileVector TargetDirection;
private:
};

