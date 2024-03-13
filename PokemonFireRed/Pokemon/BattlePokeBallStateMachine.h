#pragma once
#include "BattleStateMachine.h"

class ABattlePokeBallStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		BallUseMessage,
		PokeBallThrow,
		PokeBallBlocked,
		DontBeAThiefMessage,
		PokeBallVerticalMove,
		TestCatch,
		CatchResultAnim,
		CatchFailMessage,
		CatchSuccessMessage,
		End,
	};
public:
	// constructor destructor
	ABattlePokeBallStateMachine();
	~ABattlePokeBallStateMachine();

	// delete Function
	ABattlePokeBallStateMachine(const ABattlePokeBallStateMachine& _Other) = delete;
	ABattlePokeBallStateMachine(ABattlePokeBallStateMachine&& _Other) noexcept = delete;
	ABattlePokeBallStateMachine& operator=(const ABattlePokeBallStateMachine& _Other) = delete;
	ABattlePokeBallStateMachine& operator=(ABattlePokeBallStateMachine&& _Other) noexcept = delete;

	void Start();

	bool IsEnd() const;

protected:

private:
	void Tick(float _DeltaTime) override;

	ESubstate State = ESubstate::None;

	const float BallThrowTime = 0.8f;
	float Timer = 0.0f;
	FVector BallThrowVelocity;

	void ProcessBallUseMessage();
	void ProcessPokeBallThrow(float _DeltaTime);
	void ProcessPokeBallBlocked();
	void ProcessDontBeAThiefMessage();
	void ProcessPokeBallVerticalMove();
	void ProcessTestCatch();
	void ProcessCatchResultAnim();
	void ProcessCatchFailMessage();
	void ProcessCatchSuccessMessage();
};

