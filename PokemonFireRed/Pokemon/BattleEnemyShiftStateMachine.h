#pragma once
#include <EngineCore/Actor.h>

class ABattleEnemyShiftStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,

		End,
	};
public:
	// constructor destructor
	ABattleEnemyShiftStateMachine();
	~ABattleEnemyShiftStateMachine();

	// delete Function
	ABattleEnemyShiftStateMachine(const ABattleEnemyShiftStateMachine& _Other) = delete;
	ABattleEnemyShiftStateMachine(ABattleEnemyShiftStateMachine&& _Other) noexcept = delete;
	ABattleEnemyShiftStateMachine& operator=(const ABattleEnemyShiftStateMachine& _Other) = delete;
	ABattleEnemyShiftStateMachine& operator=(ABattleEnemyShiftStateMachine&& _Other) noexcept = delete;

protected:

private:

};

