#pragma once
#include <EngineCore/Actor.h>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;

class ABattleStartStateMachine : public AActor
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
	ABattleStartStateMachine();
	~ABattleStartStateMachine();

	// delete Function
	ABattleStartStateMachine(const ABattleStartStateMachine& _Other) = delete;
	ABattleStartStateMachine(ABattleStartStateMachine&& _Other) noexcept = delete;
	ABattleStartStateMachine& operator=(const ABattleStartStateMachine& _Other) = delete;
	ABattleStartStateMachine& operator=(ABattleStartStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, const UBattler* _Player)
	{
		Canvas = _Canvas;
		Player = _Player;
		State = ESubstate::FadeWait;
		Timer = FadeWaitTime;
	}

protected:

private:
	ABattleCanvas* Canvas = nullptr;
	const UBattler* Player = nullptr;
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

