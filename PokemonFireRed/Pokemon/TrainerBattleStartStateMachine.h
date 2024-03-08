#pragma once
#include <EngineCore/Actor.h>
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ATrainerBattleStartStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		FadeWait,
		GroundMove,
		EntryArrowMove,
		EntryBallMove,
		EnemyPokemonAppear,
		EnemyPokemonBoxMove,
		PlayerBattlerThrow,
		PlayerPokemonTakeout,
		PlayerPokemonBoxMove,
		End
	};
public:
	// constructor destructor
	ATrainerBattleStartStateMachine();
	~ATrainerBattleStartStateMachine();

	// delete Function
	ATrainerBattleStartStateMachine(const ATrainerBattleStartStateMachine& _Other) = delete;
	ATrainerBattleStartStateMachine(ATrainerBattleStartStateMachine&& _Other) noexcept = delete;
	ATrainerBattleStartStateMachine& operator=(const ATrainerBattleStartStateMachine& _Other) = delete;
	ATrainerBattleStartStateMachine& operator=(ATrainerBattleStartStateMachine&& _Other) noexcept = delete;

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
	float EnemyArrowMoveTime = 0.5f;
	float EnemyPokemonBoxMoveTime = 0.5f;
	float PlayerBattleThrowTime = 1.1f;
	float BallThrowMotionWaitTime = 0.5f;
	bool BallThrowAnimationPlayed = false;
	float PlayerPokemonTakeoutTime = 0.3f;
	float PlayerPokemonBoxMoveTime = 0.5f;

	void Tick(float _DeltaTime) override;

	void ProcessFadeWait();
	void ProcessGroundMove();
	void ProcessEnemyArrowMove();
	void ProcessEnemyBallMove();
	void ProcessEnemyPokemonAppear();
	void ProcessEnemyPokemonBoxMove();
	void ProcessPlayerBattlerThrow();
	void ProcessPlayerPokemonTakeout();
	void ProcessPlayerPokemonBoxMove();
};

