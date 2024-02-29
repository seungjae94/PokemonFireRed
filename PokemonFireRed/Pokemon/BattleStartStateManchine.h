#pragma once
#include <EngineCore/Actor.h>
#include "BattleLevel.h"

class ABattleStartStateManchine : public AActor
{
private:

public:
	// constructor destructor
	ABattleStartStateManchine();
	~ABattleStartStateManchine();

	// delete Function
	ABattleStartStateManchine(const ABattleStartStateManchine& _Other) = delete;
	ABattleStartStateManchine(ABattleStartStateManchine&& _Other) noexcept = delete;
	ABattleStartStateManchine& operator=(const ABattleStartStateManchine& _Other) = delete;
	ABattleStartStateManchine& operator=(ABattleStartStateManchine&& _Other) noexcept = delete;

protected:

private:

};

