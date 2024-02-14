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

protected:
	void RegisterEvents() override;
private:
	UImageRenderer* Renderer = nullptr;
	std::string TargetMapName;
	FTileVector TargetPoint;
	FTileVector MoveDirection;

	bool Event0();
	bool Event1();
	bool Event2();
	bool Event3();
	bool Event4();
	bool Event5();

	bool CheckPlayerDirection();
};

