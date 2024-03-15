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
		PokeBallBlockedMessage1,
		PokeBallBlockedMessage2,
		PokeBallBlockedMessage3,
		PokeBallBlockedMessage4,
		PokeBallPullInPokemon,
		PokeBallClosing,
		PokeBallDrop,
		PokeBallCheckBounceMore,
		PokeBallBounce,
		CalcCatch,
		CheckShakeMore,
		Shake,
		CatchFailAnim,
		CatchFailMessage1,
		CatchFailMessage2,
		CatchSuccessAnim,
		CatchSuccessFanfare,
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

	const float BallThrowTime = 0.65f;
	const float PullInTime = 0.5f;
	const float ClosingTime = 0.25f;
	float Timer = 0.0f;
	FVector BallVelocity;
	float BallGroundY = -90.0f * Global::FloatPixelSize;
	
	const FVector ThrowVelocity = UPokemonUtil::PixelVector(245, -350);
	const int Gravity = 700;
	const float Elasticity = 0.75f;
	const float BounceWaitTime = 0.05f;
	const int MaxBounceCount = 3;
	int BounceCount = 3;

	int EffectiveCatchRate = 0;
	int MaxTestSuccessCount = 4;
	int TestSuccessCount = 0;
	int ShakeCount = 0;
	const float CalcCatchWaitTime = 1.0f;
	const float ShakeTime = 1.75f;
	const float CatchSuccessAnimTime = 1.0f;
	const float CatchFailAnimTime = 0.15f;
	const float CatchFailMessageShowTime = 0.5f;
	const float CatchFanfareMuteTime = 3.5f;

	// 트레이너 볼 블로킹
	const float BlockTime = 0.5f;
	const float BlockMessageShowTime = 0.75f;

	void ProcessBallUseMessage();
	void ProcessPokeBallThrow(float _DeltaTime);
	void ProcessPokeBallBlocked(float _DeltaTime);
	void ProcessPokeBallBlockedMessage1();
	void ProcessPokeBallBlockedMessage2();
	void ProcessPokeBallBlockedMessage3();
	void ProcessPokeBallBlockedMessage4();
	void ProcessPokeBallPullInPokemon();
	void ProcessPokeBallClosing();
	void ProcessPokeBallDrop(float _DeltaTime);
	void ProcessPokeBallCheckBounceMore();
	void ProcessPokeBallBounce(float _DeltaTime);
	void ProcessCalcCatch();
	void ProcessCheckShakeMore();
	void ProcessShake();
	void ProcessCatchFailAnim();
	void ProcessCatchFailMessage1();
	void ProcessCatchFailMessage2();
	void ProcessCatchSuccessAnim(float _DeltaTime);
	void ProcessCatchSuccessFanfare();
	void ProcessCatchSuccessMessage();
};

