#pragma once
#include <EngineCore/Actor.h>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;
class APokemonMsgBox;

class AWildWildBattleStartStateMachine : public AActor
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
	AWildWildBattleStartStateMachine();
	~AWildWildBattleStartStateMachine();

	// delete Function
	AWildWildBattleStartStateMachine(const AWildWildBattleStartStateMachine& _Other) = delete;
	AWildWildBattleStartStateMachine(AWildWildBattleStartStateMachine&& _Other) noexcept = delete;
	AWildWildBattleStartStateMachine& operator=(const AWildWildBattleStartStateMachine& _Other) = delete;
	AWildWildBattleStartStateMachine& operator=(AWildWildBattleStartStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, const UBattler* _Player, const UBattler* _Enemy)
	{
		Canvas = _Canvas;
		MsgBox = _MsgBox;
		Player = _Player;
		Enemy = _Enemy;
		State = ESubstate::FadeWait;
		Timer = FadeWaitTime;
	}

protected:

private:
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	const UBattler* Player = nullptr;
	const UBattler* Enemy = nullptr;
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

	void ProcessFadeWait(float _DeltaTime);
	void ProcessGroundMove(float _DeltaTime);
	void ProcessEnemyPokemonBoxMove(float _DeltaTime);
	void ProcessZClickWait(float _DeltaTime);
	void ProcessPlayerBattlerThrow(float _DeltaTime);
	void ProcessPlayerPokemonTakeout(float _DeltaTime);
	void ProcessPlayerPokemonBoxMove(float _DeltaTime);
};

