#pragma once
#include <EngineCore/Actor.h>
#include "BattleCanvas.h"
#include "Battler.h"

class BattleEndStateMachine : public AActor
{
public:
	// constructor destructor
	BattleEndStateMachine();
	~BattleEndStateMachine();

	// delete Function
	BattleEndStateMachine(const BattleEndStateMachine& _Other) = delete;
	BattleEndStateMachine(BattleEndStateMachine&& _Other) noexcept = delete;
	BattleEndStateMachine& operator=(const BattleEndStateMachine& _Other) = delete;
	BattleEndStateMachine& operator=(BattleEndStateMachine&& _Other) noexcept = delete;

	void Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy);

protected:

private:

};

