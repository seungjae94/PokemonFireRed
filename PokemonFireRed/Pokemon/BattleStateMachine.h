#pragma once
#include <EngineCore/Actor.h>

class ABattleCanvas;
class APokemonMsgBox;
class AAnimatorGenerator;
class UBattler;

class ABattleStateMachine : public AActor
{
public:
	// constructor destructor
	ABattleStateMachine();
	~ABattleStateMachine();

	// delete Function
	ABattleStateMachine(const ABattleStateMachine& _Other) = delete;
	ABattleStateMachine(ABattleStateMachine&& _Other) noexcept = delete;
	ABattleStateMachine& operator=(const ABattleStateMachine& _Other) = delete;
	ABattleStateMachine& operator=(ABattleStateMachine&& _Other) noexcept = delete;

protected:
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	AAnimatorGenerator* AnimatorGenerator = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	virtual void Start();
private:

};

