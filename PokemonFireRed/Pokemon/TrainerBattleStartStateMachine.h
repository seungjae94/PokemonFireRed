#pragma once
#include "BattleStateMachine.h"
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class ABattleCanvas;
class APokemonMsgBox;

class ATrainerBattleStartStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		FadeWait,
		GroundMove,
		EntryArrowMove,
		EntryBallMove,
		ZClickWait,
		EnemyPokemonAppear,
		EnemyPokemonBoxMove,
		PlayerStartWait,
		PlayerBattlerThrow1,
		PlayerBattlerThrow2,
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

	void Start() override;

protected:

private:
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	const float FadeWaitTime = 0.5f;
	const float GroundMoveTime = 1.5f;
	const float EnemyArrowMoveTime = 0.5f;
	const float EntryBallMoveTime = 0.5f;
	const float EntryBallMoveInterval = 0.1f;
	int MovingBallIndex = 0;
	std::vector<float> BallTimers;

	const float EntryFadeTime = 0.5f;
	const float EnemyPokemonAppearTime = 0.25f;
	float EntryFadeTimer = 0.0f;
	float EnemyPokemonAppearTimer = 0.0f;

	const float EnemyPokemonBoxMoveTime = 0.5f;
	const float PlayerStartWaitTime = 0.5f;
	const float PlayerBattlerHideTime = 1.1f;
	const float PlayerPokemonTakeoutTime = 0.3f;
	const float PlayerPokemonBoxMoveTime = 0.5f;

	void Tick(float _DeltaTime) override;

	void ProcessFadeWait();
	void ProcessGroundMove();
	void ProcessEntryArrowMove();
	void ProcessEntryBallMove(float _DeltaTime);
	void ProcessZClickWait();
	void ProcessEnemyPokemonAppear(float _DeltaTime);
	void ProcessEnemyPokemonBoxMove();
	void ProcessPlayerStartWait();
	void ProcessPlayerBattlerThrow1(float _DeltaTime);
	void ProcessPlayerBattlerThrow2(float _DeltaTime);
	void ProcessPlayerPokemonTakeout();
	void ProcessPlayerPokemonBoxMove();
};

