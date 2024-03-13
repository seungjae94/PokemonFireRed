#pragma once
#include "BattleStateMachine.h"
#include "Global.h"
#include "PokemonUtil.h"

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
		PokeBallPullInPokemon,
		PokeBallClosing,
		PokeBallDrop,
		PokeBallCheckBounceMore,
		PokeBallBounce,
		CalcCatch,
		CheckShakeMore,
		Shake,
		CatchFailAnim,
		CatchSuccessAnim,
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
	const float PullInTime = 0.5f;
	const float ClosingTime = 0.25f;
	float Timer = 0.0f;
	FVector BallVelocity;
	float BallGroundY = -90.0f * Global::FloatPixelSize;
	
	const FVector ThrowVelocity = UPokemonUtil::PixelVector(200, -250);
	const int Gravity = 400;
	const float Elasticity = 0.75f;
	const float BounceWaitTime = 0.05f;
	const int MaxBounceCount = 3;
	int BounceCount = 3;

	int EffectiveCatchRate = 0;
	int MaxTestSuccessCount = 4;
	int TestSuccessCount = 0;
	int ShakeCount = 0;

	void ProcessBallUseMessage();
	void ProcessPokeBallThrow(float _DeltaTime);
	void ProcessPokeBallBlocked();
	void ProcessDontBeAThiefMessage();
	void ProcessPokeBallPullInPokemon();
	void ProcessPokeBallClosing();
	void ProcessPokeBallDrop(float _DeltaTime);
	void ProcessPokeBallCheckBounceMore();
	void ProcessPokeBallBounce(float _DeltaTime);
	void ProcessCalcCatch();
	void ProcessCheckShakeMore();
	void ProcessShake();
	void ProcessCatchFailAnim();
	void ProcessCatchSuccessAnim();
};

