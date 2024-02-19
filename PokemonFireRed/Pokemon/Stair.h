#pragma once
#include "Warp.h"

class AStair : public AWarp
{
public:
	// constructor destructor
	AStair();
	~AStair();

	// delete Function
	AStair(const AStair& _Other) = delete;
	AStair(AStair&& _Other) noexcept = delete;
	AStair& operator=(const AStair& _Other) = delete;
	AStair& operator=(AStair&& _Other) noexcept = delete;

	void SetFirstPath(const std::vector<FVector>& _FirstPath)
	{
		FirstPath = _FirstPath;
	}

	void SetSecondPath(const std::vector<FVector>& _SecondPath)
	{
		SecondPath = _SecondPath;
	}

	void RegisterPredefinedEvent() override;

protected:

private:
	std::vector<FVector> FirstPath;
	std::vector<FVector> SecondPath;
};

