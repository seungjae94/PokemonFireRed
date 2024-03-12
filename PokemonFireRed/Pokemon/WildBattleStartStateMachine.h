#pragma once
#include "BattleStateMachine.h"
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;
class APokemonMsgBox;

class AWildBattleStartStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		FadeWait,
		GroundMove,
		EnemyPokemonBoxMove,
		ZClickWait,
		PlayerBattlerThrow,
		PlayerPokemonTakeout,
		PlayerPokemonBoxMove,
		End
	};
public:
	// constructor destructor
	AWildBattleStartStateMachine();
	~AWildBattleStartStateMachine();

	// delete Function
	AWildBattleStartStateMachine(const AWildBattleStartStateMachine& _Other) = delete;
	AWildBattleStartStateMachine(AWildBattleStartStateMachine&& _Other) noexcept = delete;
	AWildBattleStartStateMachine& operator=(const AWildBattleStartStateMachine& _Other) = delete;
	AWildBattleStartStateMachine& operator=(AWildBattleStartStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start() override;

protected:

private:
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	float FadeWaitTime = 0.5f;
	float GroundMoveTime = 1.5f;
	float EnemyPokemonBoxMoveTime = 0.5f;
	float PlayerBattleThrowTime = 1.1f;
	float BallThrowMotionWaitTime = 0.5f;
	bool BallThrowAnimationPlayed = false;
	float PlayerPokemonTakeoutTime = 0.3f;
	float PlayerPokemonBoxMoveTime = 0.5f;

	void Tick(float _DeltaTime) override;

	void ProcessFadeWait();
	void ProcessGroundMove();
	void ProcessEnemyPokemonBoxMove();
	void ProcessZClickWait();
	void ProcessPlayerBattlerThrow();
	void ProcessPlayerPokemonTakeout();
	void ProcessPlayerPokemonBoxMove();
};

