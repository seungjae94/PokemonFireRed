#pragma once
#include <vector>
#include "PokemonMath.h"
#include "EventEnums.h"

class AEventTarget;
class ASurprise;

struct FMoveBaseData
{
	bool bIsMoving = false;
	std::vector<AEventTarget*> MoveTargets;
	std::vector<FTileVector> MovePrevPoints;
	std::vector<FTileVector> MoveNextPoints;
	std::vector<bool> MoveEnds;
	int MovePathIndex = 0;
	float MoveTime = 0.0f;
	float MoveTimer = 0.0f;
};

struct FMoveDynamicPathData
{
	std::vector<std::vector<FTileVector>> DynamicPaths;
};

struct FMoveWRData
{
	FVector MoveWRPrevPos;
	FVector MoveWRNextPos;
	int MoveWRPathIndex = -1;
	float MoveWRTime = 0.0f;
	float MoveWRTimer = 0.0f;
};

struct FSurpriseData
{
	bool SurpriseFirstTick = true;
	ASurprise* SurpriseIcon = nullptr;
};

struct FWaitData
{
	bool IsBegin = true;
	float Timer = 0.0f;
};